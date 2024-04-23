#pragma once
#include <vector>
#include <QString>
#include "ChessPiece.h"


// 棋盘上一点的状态
enum class ChessBoardPointState
{
    Empty,      // 空
    OurSide,    // 我方
    OtherSide   // 对方
};

struct ChessBoardPoint
{
    ChessBoardPointState state;     // 棋子状态
    ChessPieceType type;            // 棋子类型
};

struct Move
{
    int initX, initY;
    int nextX, nextY;
};

// 将字符转换为棋盘上一点
ChessBoardPoint char2ChessBoardPoint(char c);

// 将棋盘上一点转换为字符
char chessBoardPoint2Char(const ChessBoardPoint& point);

// 棋盘
// x坐标向右，y坐标向下；假设己方在下
class ChessBoard
{
private:
    ChessBoardPoint board[10][9];       // 棋盘
    bool bOurTurn;                      // 是否是己方回合
    std::vector<Move> possibleMoves;    // 当前所有可能的行动
    bool bGameOver{};                   // 是否已经结束
    bool bOurVictory{};                 // 己方是否获胜

    // 生成车可能的行动
    std::vector<Move> generateRookMoves(int x, int y) const;
    // 生成马可能的行动
    std::vector<Move> generateKnightMoves(int x, int y) const;
    // 生成相可能的行动
    std::vector<Move> generateBishopMoves(int x, int y) const;
    // 生成士可能的行动
    std::vector<Move> generateAdvisorMoves(int x, int y) const;
    // 生成将可能的行动
    std::vector<Move> generateKingMoves(int x, int y) const;
    // 生成炮可能的行动
    std::vector<Move> generateCannonMoves(int x, int y) const;
    // 生成兵可能的行动
    std::vector<Move> generatePawnMoves(int x, int y) const;
    // 生成某个棋子可能的行动
    std::vector<Move> generateMovesForChessPiece(int x, int y) const;
    // 更新当前可能的行动，以及是否已经结束
    void updatePossibleMoves();
    // 评估棋子对优势函数的贡献
    int evaluateChessPieceAdvantage(int x, int y) const;
    // 是否已经有其中一个将被吃掉；如果有，isOurVictory返回是否是我方获胜
    bool isOneOfKingsCaptured(bool& isOurVictory) const;

public:
    // 从文件中读取棋盘
    ChessBoard(const QString& filePath, bool _bOurTurn = true);
    // 打印棋盘
    void print() const;
    // 将棋子坐标转换到对方视角
    static void transformCoordinates(int& x, int& y);
    // 将行动转换到对方视角
    static void transformMove(Move& move);
    // 是否是我方回合
    bool isOurTurn() const;
    // 评估当前棋局中我方的优势
    int evaluateAdvantage() const;
    // 返回当前所有可能的行动，若游戏已结束则返回空
    const std::vector<Move>& getAllPossibleMoves() const;
    // 游戏是否已经结束（如果任意将被吃掉或者当前方无法行动）
    bool isGameOver() const;
    // 己方是否获胜；该函数只有在游戏结束后才能调用
    bool isOurVictory() const;
    // 生成采取一个行动之后的棋盘
    ChessBoard takeAction(const Move& move) const;
    // 生成跳过当前回合的棋盘（用于debug）
    ChessBoard skipCurrentTurn() const;
    // 查询棋盘上的一点
    ChessBoardPoint getChessBoardPoint(int x, int y) const;
    // 生成对方视角的棋盘
    ChessBoard getOpponentView() const;
    // 当前行动方是否被将军
    bool isCheck() const;
    // 是否是有效的行动
    bool isValidMove(const Move& move) const;
};
