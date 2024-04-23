#include "Worker.h"
#include "Policy.h"


Worker::Worker(const ChessBoard& _chessBoard, Move& _bestMove, QObject* parent)
    : QObject(parent), chessBoard(_chessBoard), bestMove(_bestMove)
{}


Worker::~Worker()
{}

void Worker::process()
{
    // 计算对方最佳行动
    /*
    * 这里注意到一点，因为采用alpha-beta剪枝算法，这个算法假设对方总是采取最优行动
    * 因此如果搜索的深度太深，可能会发现自己已经输掉；但实际上，对方可能并不会采取最优行动
    * 所以，在发现自己必输的情况下，减小搜索深度再次尝试
    */

    int maxScore;
    int initSearchDepth = 4;
    for (int searchDepth = initSearchDepth; searchDepth >= 2; searchDepth--)
    {
        bestMove = getOpponentBestMoveMultiThread(chessBoard, &maxScore, searchDepth);
        if (maxScore != -100000) break;
    }

    emit finished();
}
