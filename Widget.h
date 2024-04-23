#pragma once
#include <QWidget>
#include "ui_Widget.h"
#include <QMap>
#include "ChessBoard.h"

struct Coordinate
{
    int x;
    int y;
    bool operator<(const Coordinate& other) const
    {
        return x < other.x || (x == other.x && y < other.y);
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetClass; };
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

protected:
    // 当玩家点击了鼠标左键
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    // 对方开始思考
    void startThinking();
    // 对方思考结束
    void endThinking();

private:
    Ui::WidgetClass* ui;
    ChessBoard chessBoard;

    // 注意到移动棋子时也需要改变chessPieces
    QMap<Coordinate, QLabel*> chessPieces;
    QLabel* selectionBox;
    // 棋盘左上角的坐标（像素）
    int chessBoardLeftUpX = 75;
    int chessBoadLeftUpY = 43;
    // 棋盘上一个方格的宽度（像素）
    float squareWidth = 81.25;
    // 棋盘上一个方格的高度（像素）
    float squareHeight = 79;
    // 玩家当前是否可以移动棋子
    bool bCanMove{ true };
    // 选中的棋子的坐标
    int selectedX{ -1 }, selectedY{ -1 };

    // 对方思考得到的最佳行动
    Move bestMove;

    // 根据chessBoard中的棋子，添加棋子到UI界面上
    void addChessPieces();
    // 得到棋子在UI界面上的坐标
    QPoint getChessPiecePos(int x, int y) const;
    // 得到选择框在UI界面上的坐标
    QPoint getSelectionBoxPos(int x, int y) const;
    // 移动棋子（在chessBoard中移动棋子的同时，也在UI界面上移动棋子）
    void moveChessPiece(const Move& move);
    // 将UI中的坐标转换为棋盘坐标；返回值：是否在棋盘上
    bool transformPosToChessBoardPos(int posX, int posY, int& x, int& y) const;
    // 检查是否将军、是否将死，是否将已经被吃掉；仅当游戏已经结束时，返回true
    bool checkCheckAndGameOver();
};
