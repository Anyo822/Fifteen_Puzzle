#pragma once
#include <QAbstractListModel>
#include <vector>

class GameBoard : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int dimension READ dimension CONSTANT)
    Q_PROPERTY(int hiddenElementValue READ hiddenElementValue CONSTANT)
public:
    using Position = std::pair<size_t, size_t>;
    static constexpr size_t defaultPuzzleDimension {4};
    GameBoard(QObject *parent = nullptr, size_t board_dimension = defaultPuzzleDimension);

    struct Tile {
        size_t value {};
        Tile& operator=(const size_t newValue) {
            value = newValue;
            return *this;
        }
        bool operator==(const size_t other) {
            return other == value;
        }
    };

    int rowCount(const QModelIndex& parent = QModelIndex {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE bool move(const int index);

    size_t dimension() const;
    size_t boardSize() const;
    size_t hiddenElementValue() const;
    Q_INVOKABLE void shuffle();
private:
    bool isPositionValid(const size_t position) const;
    bool isBoardValid() const;
    Position getRowCol(const size_t index) const;

    std::vector<Tile> m_rawBoard;
    const size_t m_dimension;
    const size_t m_boardSize;
    const size_t m_hiddenElementValue;
};

