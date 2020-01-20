#include "gameboard.h"
#include <algorithm>
#include <random>
#include <QDebug>

GameBoard::GameBoard(QObject *parent, size_t board_dimension)
    : QAbstractListModel {parent},
      m_dimension {board_dimension},
      m_boardSize {m_dimension * m_dimension},
      m_hiddenElementValue {m_boardSize}
{
    m_rawBoard.resize(m_boardSize);
    std::iota(m_rawBoard.begin(), m_rawBoard.end(), 1);
    shuffle();
}

int GameBoard::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rawBoard.size();
}

QVariant GameBoard::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return {};
    }

    const int rowIndex {index.row()};

    if(!isPositionValid(rowIndex)) {
        return {};
    }

    return QVariant::fromValue(m_rawBoard[rowIndex]);
}


void GameBoard::shuffle()
{
    beginResetModel();

    static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);

    do {
        std::shuffle(m_rawBoard.begin(), m_rawBoard.end(), generator);
    } while (!isBoardValid());

    endResetModel();
}

bool GameBoard::isPositionValid(const size_t position) const
{
    return position < m_boardSize;
}

bool GameBoard::isBoardValid() const
{
    int inv {0};
        for (size_t i {0}; i < m_boardSize; ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (m_rawBoard[j] > m_rawBoard[i]) {
                    ++inv;
                }
            }
        }

        const size_t start_point = 1;

        for (size_t i = 0; i < m_boardSize; ++i) {
            if (m_rawBoard[i] == m_boardSize) {
                inv += start_point + i / m_dimension;
            }
        }

        return (inv % 2) == 0;
}

GameBoard::Position GameBoard::getRowCol(size_t index) const
{
    Q_ASSERT(m_dimension > 0);
    size_t row = index / m_dimension;
    size_t column = index % m_dimension;

    return std::make_pair(row, column);
}

size_t GameBoard::hiddenElementValue() const
{
    return m_hiddenElementValue;
}

size_t GameBoard::boardSize() const
{
    return m_boardSize;
}

size_t GameBoard::dimension() const
{
    return m_dimension;
}

namespace {
    bool isAdjacent(const GameBoard::Position f, const GameBoard::Position s)
    {
        if(f == s) {
            return false;
        }

        const auto calcDistance = [](const size_t pos1, const size_t pos2) {
            return std::abs(static_cast<int>(pos1) - static_cast<int>(pos2));

//                    int distance = static_cast<int>(pos1);
//            distance -= static_cast<int>(pos2);
//            distance = std::abs(distance);
//            return distance;
        };

        return calcDistance(f.first, s.first) + calcDistance(f.second, s.second) == 1;

//        bool result {false};

//        if (f.first == s.first) {
//           int distance = calcDistance(f.second, s.second);
//           if(distance == 1) {
//               result = true;
//           }
//        } else if (f.second == s.second) {
//            int distance = calcDistance(f.first, s.first);
//            if (distance == 1) {
//                result = true;
//            }
//        }

//        return result;
    }
}

bool GameBoard::move(const int index)
{
    if (!isPositionValid(static_cast<size_t>(index))) {
        return false;
    }

    const Position elementPosition {getRowCol(index)};

    auto hiddenElementIterator = std::find(m_rawBoard.begin(), m_rawBoard.end(), boardSize());
    Q_ASSERT(hiddenElementIterator != m_rawBoard.end());

    Position hiddenElementPosition (getRowCol(std::distance(m_rawBoard.begin(), hiddenElementIterator)));

    if(!isAdjacent(elementPosition, hiddenElementPosition)) {
        return false;
    }

    int emptyTile = std::distance(m_rawBoard.begin(), hiddenElementIterator);
    int clickedTile = std::distance(m_rawBoard.begin(), std::find(m_rawBoard.begin(), m_rawBoard.end(), m_rawBoard[index]));
    int moveDirection = emptyTile - clickedTile;

    //beginmoverows

    if (moveDirection > 1)
    {
        beginMoveRows(QModelIndex {}, emptyTile, emptyTile, QModelIndex {}, clickedTile);
        endMoveRows();

        beginMoveRows(QModelIndex {}, clickedTile + 1, clickedTile + 1, QModelIndex {}, emptyTile + 1);
        endMoveRows();
    }
    else if (moveDirection < -1)
    {
        beginMoveRows(QModelIndex {}, emptyTile, emptyTile, QModelIndex {}, clickedTile);
        endMoveRows();

        beginMoveRows(QModelIndex {}, clickedTile, clickedTile, QModelIndex {}, emptyTile);
        endMoveRows();
    }
    else if (moveDirection == -1)
    {
        beginMoveRows(QModelIndex {}, clickedTile, clickedTile, QModelIndex {}, emptyTile);
        endMoveRows();
    }
    else if (moveDirection == 1)
    {
        beginMoveRows(QModelIndex {}, clickedTile, clickedTile, QModelIndex {}, emptyTile + 1);
        endMoveRows();
    }

    std::swap(*hiddenElementIterator, m_rawBoard[index]);

    //emit dataChanged(createIndex(0, 0), createIndex(m_boardSize, 0));

    return true;
}


