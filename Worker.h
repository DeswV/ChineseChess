#pragma once
#include <QObject>
#include "ChessBoard.h"


// 用于在另一个线程计算对手的最佳行动
class Worker : public QObject
{
    Q_OBJECT

public:
    Worker(const ChessBoard& _chessBoard, Move& _bestMove, QObject* parent = nullptr);
    ~Worker();

public slots:
    void process();

signals:
    void finished();

private:
    const ChessBoard& chessBoard;
    Move& bestMove;
};
