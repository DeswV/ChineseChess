#include "Policy.h"
#include <iostream>
#include <cassert>
#include <climits>
#include <algorithm>
#include <vector>
#include <thread>


int alphaBeta(const ChessBoard& node, int depth, int alpha, int beta, bool isMaxTurn)
{
    assert(isMaxTurn == node.isOurTurn());

    if (node.isGameOver() || depth == 0) return node.evaluateAdvantage();

    const std::vector<Move>& moves = node.getAllPossibleMoves();
    if (isMaxTurn)
    {
        for (const auto& move : moves)
        {
            ChessBoard nextNode = node.takeAction(move);
            alpha = std::max(alpha, alphaBeta(nextNode, depth - 1, alpha, beta, false));
            if (beta <= alpha) break;
        }
        return alpha;
    }
    else
    {
        for (const auto& move : moves)
        {
            ChessBoard nextNode = node.takeAction(move);
            beta = std::min(beta, alphaBeta(nextNode, depth - 1, alpha, beta, true));
            if (beta <= alpha) break;
        }
        return beta;
    }
}

Move getBestMove(const ChessBoard& board, int* _maxScore, int searchDepth)
{
    assert(board.isOurTurn());
    assert(searchDepth >= 1);

    int maxScore = INT_MIN;
    int indexOfBestMove = -1;
    const std::vector<Move>& moves = board.getAllPossibleMoves();
    if (moves.empty()) return { -1,-1,-1,-1 };

    for (int i{}; i < moves.size(); i++)
    {
        const auto& move = moves[i];

        ChessBoard nextNode = board.takeAction(move);
        int score = alphaBeta(nextNode, searchDepth - 1, INT_MIN, INT_MAX, false);
        if (score > maxScore)
        {
            maxScore = score;
            indexOfBestMove = i;
        }
    }

    if (_maxScore) *_maxScore = maxScore;

    return moves[indexOfBestMove];
}

Move getOpponentBestMove(const ChessBoard& board, int* _maxScore, int searchDepth)
{
    ChessBoard opponentView = board.getOpponentView();
    Move bestMove = getBestMove(opponentView, _maxScore, searchDepth);
    ChessBoard::transformMove(bestMove);
    return bestMove;
}

bool isCheckmate(const ChessBoard& board)
{
    int maxScore;
    // 如果两步以内必输，那么就是将死
    if (board.isOurTurn()) {
        getBestMoveMultiThread(board, &maxScore, 2);
    }
    else {
        getOpponentBestMoveMultiThread(board, &maxScore, 2);
    }
    return maxScore == -100000;
}

void _alpha_beta(const ChessBoard& node, int depth, int alpha, int beta, bool isMaxTurn, int& score)
{
    score = alphaBeta(node, depth, alpha, beta, isMaxTurn);
}

Move getBestMoveMultiThread(const ChessBoard& board, int* _maxScore, int searchDepth)
{
    assert(board.isOurTurn());
    assert(searchDepth >= 1);

    const std::vector<Move>& moves = board.getAllPossibleMoves();
    if (moves.empty()) return { -1,-1,-1,-1 };

    // 准备启动多线程并行处理
    std::vector<ChessBoard> childrenNodes;
    std::vector<int> scores;
    for (const auto& move : moves)
    {
        ChessBoard nextNode = board.takeAction(move);
        childrenNodes.push_back(nextNode);
    }
    scores.resize(moves.size());
    std::vector<std::thread> threads;
    
    for (int i{}; i < moves.size(); i++)
    {
        threads.push_back(std::thread(_alpha_beta, std::ref(childrenNodes[i]), searchDepth - 1, INT_MIN, INT_MAX, false, std::ref(scores[i])));
    }
    // 等待所有线程结束
    for (auto& thread : threads)
    {
        thread.join();
    }

    // 选择最大的分数
    int maxScore = INT_MIN;
    int indexOfBestMove = -1;
    for (int i{}; i < moves.size(); i++)
    {
        if (scores[i] > maxScore)
        {
            maxScore = scores[i];
            indexOfBestMove = i;
        }
    }

    if (_maxScore) *_maxScore = maxScore;

    return moves[indexOfBestMove];
}

Move getOpponentBestMoveMultiThread(const ChessBoard& board, int* _maxScore, int searchDepth)
{
    ChessBoard opponentView = board.getOpponentView();
    Move bestMove = getBestMoveMultiThread(opponentView, _maxScore, searchDepth);
    ChessBoard::transformMove(bestMove);
    return bestMove;
}
