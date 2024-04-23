#include "ChessBoard.h"
#include <cassert>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "BoardEvaluation.h"


ChessBoardPoint char2ChessBoardPoint(char c)
{
    ChessBoardPoint ret;

    switch (c)
    {
    case 'r':
        ret.state = ChessBoardPointState::OtherSide;
        ret.type = ChessPieceType::Rook;
        break;
    case 'R':
        ret.state = ChessBoardPointState::OurSide;
        ret.type = ChessPieceType::Rook;
        break;
    case 'n':
        ret.state = ChessBoardPointState::OtherSide;
        ret.type = ChessPieceType::kNight;
        break;
    case 'N':
        ret.state = ChessBoardPointState::OurSide;
        ret.type = ChessPieceType::kNight;
        break;
    case 'b':
        ret.state = ChessBoardPointState::OtherSide;
        ret.type = ChessPieceType::Bishop;
        break;
    case 'B':
        ret.state = ChessBoardPointState::OurSide;
        ret.type = ChessPieceType::Bishop;
        break;
    case 'a':
        ret.state = ChessBoardPointState::OtherSide;
        ret.type = ChessPieceType::Advisor;
        break;
    case 'A':
        ret.state = ChessBoardPointState::OurSide;
        ret.type = ChessPieceType::Advisor;
        break;
    case 'k':
        ret.state = ChessBoardPointState::OtherSide;
        ret.type = ChessPieceType::King;
        break;
    case 'K':
        ret.state = ChessBoardPointState::OurSide;
        ret.type = ChessPieceType::King;
        break;
    case 'c':
        ret.state = ChessBoardPointState::OtherSide;
        ret.type = ChessPieceType::Cannon;
        break;
    case 'C':
        ret.state = ChessBoardPointState::OurSide;
        ret.type = ChessPieceType::Cannon;
        break;
    case 'p':
        ret.state = ChessBoardPointState::OtherSide;
        ret.type = ChessPieceType::Pawn;
        break;
    case 'P':
        ret.state = ChessBoardPointState::OurSide;
        ret.type = ChessPieceType::Pawn;
        break;
    case '.':
        ret.state = ChessBoardPointState::Empty;
        break;
    default:
        qCritical() << "Invalid character: " << c;
        exit(2);
    }

    return ret;
}

char chessBoardPoint2Char(const ChessBoardPoint& point)
{
    switch (point.state)
    {
    case ChessBoardPointState::Empty:
        return '.';
    case ChessBoardPointState::OurSide:
        switch (point.type)
        {
        case ChessPieceType::Rook:
            return 'R';
        case ChessPieceType::kNight:
            return 'N';
        case ChessPieceType::Bishop:
            return 'B';
        case ChessPieceType::Advisor:
            return 'A';
        case ChessPieceType::King:
            return 'K';
        case ChessPieceType::Cannon:
            return 'C';
        case ChessPieceType::Pawn:
            return 'P';
        default:
            qCritical() << "Invalid chess piece type";
            exit(2);
        }
    case ChessBoardPointState::OtherSide:
        switch (point.type)
        {
        case ChessPieceType::Rook:
            return 'r';
        case ChessPieceType::kNight:
            return 'n';
        case ChessPieceType::Bishop:
            return 'b';
        case ChessPieceType::Advisor:
            return 'a';
        case ChessPieceType::King:
            return 'k';
        case ChessPieceType::Cannon:
            return 'c';
        case ChessPieceType::Pawn:
            return 'p';
        default:
            qCritical() << "Invalid chess piece type";
            exit(2);
        }
    default:
        qCritical() << "Invalid chess board point state";
        exit(2);
    }
}

ChessBoard::ChessBoard(const QString& filePath, bool _bOurTurn) :
    bOurTurn(_bOurTurn)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file for reading: " << qPrintable(file.errorString());
        exit(1);
    }

    QTextStream in(&file);

    char c;
    for (int i{}; i < 10; ++i)
    {
        for (int j{}; j < 9; ++j)
        {
            in >> c;
            board[i][j] = char2ChessBoardPoint(c);
        }
        // 读取换行符
        in >> c;
    }

    updatePossibleMoves();
}

void ChessBoard::print() const
{
    QDebug cout = qInfo().noquote().nospace();

    for (int i{}; i < 10; ++i)
    {
        for (int j{}; j < 9; ++j)
        {
            cout << chessBoardPoint2Char(board[i][j]);
        }
        cout << '\n';
    }
}

void ChessBoard::transformCoordinates(int& x, int& y)
{
    x = 8 - x;
    y = 9 - y;
}

void ChessBoard::transformMove(Move& move)
{
    transformCoordinates(move.initX, move.initY);
    transformCoordinates(move.nextX, move.nextY);
}

bool ChessBoard::isOurTurn() const
{
    return bOurTurn;
}

bool ChessBoard::isOneOfKingsCaptured(bool& isOurVictory) const
{
    bool bOurKingExist = false;
    bool bOtherKingExist = false;

    // 先在对方皇宫找一遍
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 3; x < 6; ++x)
        {
            if (board[y][x].state == ChessBoardPointState::OurSide && board[y][x].type == ChessPieceType::King)
            {
                bOurKingExist = true;
            }
            else if (board[y][x].state == ChessBoardPointState::OtherSide && board[y][x].type == ChessPieceType::King)
            {
                bOtherKingExist = true;
            }
        }
    }
    // 再在我方皇宫找一遍
    for (int y = 7; y < 10; ++y)
    {
        for (int x = 3; x < 6; ++x)
        {
            if (board[y][x].state == ChessBoardPointState::OurSide && board[y][x].type == ChessPieceType::King)
            {
                bOurKingExist = true;
            }
            else if (board[y][x].state == ChessBoardPointState::OtherSide && board[y][x].type == ChessPieceType::King)
            {
                bOtherKingExist = true;
            }
        }
    }

    if (bOurKingExist && bOtherKingExist) return false;
    else {
        isOurVictory = bOurKingExist;
        return true;
    }
}

int ChessBoard::evaluateAdvantage() const
{
    if (bGameOver)
    {
        if (bOurVictory) return 100000;
        else return -100000;
    }

    int ourAdvantage{};
    int otherAdvantage{};
    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 9; ++x)
        {
            if (board[y][x].state == ChessBoardPointState::OurSide)
            {
                ourAdvantage += evaluateChessPieceAdvantage(x, y);
            }
            else if (board[y][x].state == ChessBoardPointState::OtherSide)
            {
                otherAdvantage += evaluateChessPieceAdvantage(x, y);
            }
        }
    }
    return ourAdvantage - otherAdvantage;
}

const std::vector<Move>& ChessBoard::getAllPossibleMoves() const
{
    return possibleMoves;
}

bool ChessBoard::isGameOver() const
{
    return bGameOver;
}

bool ChessBoard::isOurVictory() const
{
    assert(bGameOver);
    return bOurVictory;
}

void ChessBoard::updatePossibleMoves()
{
    possibleMoves.clear();

    // 先检查是否已经有将被吃掉
    if (isOneOfKingsCaptured(bOurVictory))
    {
        bGameOver = true;
        return;
    }

    for (int y{}; y < 10; ++y)
    {
        for (int x{}; x < 9; ++x)
        {
            if ((bOurTurn && board[y][x].state == ChessBoardPointState::OurSide) || (!bOurTurn && board[y][x].state == ChessBoardPointState::OtherSide))
            {
                auto moves = generateMovesForChessPiece(x, y);
                possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());
            }
        }
    }

    // 如果没有任何行动可以采取，游戏结束
    if (possibleMoves.empty())
    {
        bGameOver = true;
        bOurVictory = !bOurTurn;
    }
}

ChessBoard ChessBoard::takeAction(const Move& move) const
{
    assert(!bGameOver);
    assert(board[move.initY][move.initX].state == (bOurTurn ? ChessBoardPointState::OurSide : ChessBoardPointState::OtherSide));

    ChessBoard nextChessBoard = *this;
    nextChessBoard.board[move.nextY][move.nextX] = nextChessBoard.board[move.initY][move.initX];
    nextChessBoard.board[move.initY][move.initX].state = ChessBoardPointState::Empty;
    nextChessBoard.bOurTurn = !nextChessBoard.bOurTurn;
    nextChessBoard.updatePossibleMoves();
    return nextChessBoard;
}

ChessBoard ChessBoard::skipCurrentTurn() const
{
    assert(!bGameOver);

    ChessBoard nextChessBoard = *this;
    nextChessBoard.bOurTurn = !nextChessBoard.bOurTurn;
    nextChessBoard.updatePossibleMoves();
    return nextChessBoard;
}

ChessBoardPoint ChessBoard::getChessBoardPoint(int x, int y) const
{
    return board[y][x];
}

ChessBoard ChessBoard::getOpponentView() const
{
    ChessBoard opponentView = *this;

    for (int y{}; y < 10; y++)
    {
        for (int x{}; x < 9; x++)
        {
            int tX = x, tY = y;
            transformCoordinates(tX, tY);
            opponentView.board[tY][tX] = board[y][x];
            if (opponentView.board[tY][tX].state != ChessBoardPointState::Empty)
            {
                opponentView.board[tY][tX].state = (opponentView.board[tY][tX].state == ChessBoardPointState::OurSide ? ChessBoardPointState::OtherSide : ChessBoardPointState::OurSide);
            }
        }
    }

    opponentView.bOurTurn = !bOurTurn;

    for (auto& move : opponentView.possibleMoves)
    {
        transformMove(move);
    }

    if (bGameOver)
    {
        opponentView.bOurVictory = !bOurVictory;
    }

    return opponentView;
}

bool ChessBoard::isCheck() const
{
    for (int y{}; y < 10; y++)
    {
        for (int x{}; x < 9; x++)
        {
            if ((board[y][x].state == ChessBoardPointState::OtherSide && bOurTurn) || (board[y][x].state == ChessBoardPointState::OurSide && !bOurTurn))
            {
                auto moves = generateMovesForChessPiece(x, y);
                for (const auto& move : moves)
                {
                    if (board[move.nextY][move.nextX].state == (bOurTurn ? ChessBoardPointState::OurSide : ChessBoardPointState::OtherSide) && board[move.nextY][move.nextX].type == ChessPieceType::King)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool ChessBoard::isValidMove(const Move& move) const
{
    for (const auto& possibleMove : possibleMoves)
    {
        if (possibleMove.initX == move.initX && possibleMove.initY == move.initY && possibleMove.nextX == move.nextX && possibleMove.nextY == move.nextY)
        {
            return true;
        }
    }
    return false;
}

// 对应于上下左右移动一步的x、y偏移值
const int deltaXs[] = { 0, 0, -1, 1 };
const int deltaYs[] = { -1, 1, 0, 0 };

std::vector<Move> ChessBoard::generateRookMoves(int x, int y) const
{
    assert(board[y][x].type == ChessPieceType::Rook);
    ChessBoardPointState state = board[y][x].state;
    assert(state == ChessBoardPointState::OurSide || state == ChessBoardPointState::OtherSide);

    std::vector<Move> ret;

    for (int dir = 0; dir < 4; ++dir) {
        int deltaX = deltaXs[dir];
        int deltaY = deltaYs[dir];
        for (int x_i = x + deltaX, y_i = y + deltaY; x_i >= 0 && x_i < 9 && y_i >= 0 && y_i < 10; x_i += deltaX, y_i += deltaY)
        {
            if (board[y_i][x_i].state == ChessBoardPointState::Empty)
            {
                ret.push_back({ x, y, x_i, y_i });
            }
            else if (board[y_i][x_i].state != state)
            {
                ret.push_back({ x, y, x_i, y_i });
                break;
            }
            else
            {
                break;
            }
        }
    }

    return ret;
}

std::vector<Move> ChessBoard::generateKnightMoves(int x, int y) const
{
    assert(board[y][x].type == ChessPieceType::kNight);
    ChessBoardPointState state = board[y][x].state;
    assert(state == ChessBoardPointState::OurSide || state == ChessBoardPointState::OtherSide);

    static const int deltaXsForKnight[] = { -2, -1, 1, 2, 2, 1, -1, -2 };
    static const int deltaYsForKnight[] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    // 马可能会被障碍所阻挡
    static const int deltaXsForKnightObstacle[] = { -1, 0, 0, 1, 1, 0, 0, -1 };
    static const int deltaYsForKnightObstacle[] = { 0, 1, 1, 0, 0, -1, -1, 0 };

    std::vector<Move> ret;

    for (int dir = 0; dir < 8; dir++)
    {
        int x_i = x + deltaXsForKnight[dir];
        int y_i = y + deltaYsForKnight[dir];
        int x_obstacle = x + deltaXsForKnightObstacle[dir];
        int y_obstacle = y + deltaYsForKnightObstacle[dir];
        if (x_i >= 0 && x_i < 9 && y_i >= 0 && y_i < 10 && x_obstacle >= 0 && x_obstacle < 9 && y_obstacle >= 0 && y_obstacle < 10)
        {
            if (board[y_i][x_i].state != state && board[y_obstacle][x_obstacle].state == ChessBoardPointState::Empty)
            {
                ret.push_back({ x, y, x_i, y_i });
            }
        }
    }

    return ret;
}

std::vector<Move> ChessBoard::generateBishopMoves(int x, int y) const
{
    assert(board[y][x].type == ChessPieceType::Bishop);
    ChessBoardPointState state = board[y][x].state;
    assert(state == ChessBoardPointState::OurSide || state == ChessBoardPointState::OtherSide);

    static const int deltaXsForBishop[] = { -2, -2, 2, 2 };
    static const int deltaYsForBishop[] = { -2, 2, 2, -2 };
    // 相可能会被障碍所阻挡
    static const int deltaXsForBishopObstacle[] = { -1, -1, 1, 1 };
    static const int deltaYsForBishopObstacle[] = { -1, 1, 1, -1 };

    std::vector<Move> ret;

    // 确定移动范围
    int yMin, yMax;
    if (state == ChessBoardPointState::OurSide)
    {
        yMin = 5;
        yMax = 9;
    }
    else
    {
        yMin = 0;
        yMax = 4;
    }

    for (int dir = 0; dir < 4; dir++)
    {
        int x_i = x + deltaXsForBishop[dir];
        int y_i = y + deltaYsForBishop[dir];
        int x_obstacle = x + deltaXsForBishopObstacle[dir];
        int y_obstacle = y + deltaYsForBishopObstacle[dir];
        if (x_i >= 0 && x_i < 9 && y_i >= yMin && y_i <= yMax)
        {
            if (board[y_i][x_i].state != state && board[y_obstacle][x_obstacle].state == ChessBoardPointState::Empty)
            {
                ret.push_back({ x, y, x_i, y_i });
            }
        }
    }

    return ret;
}

std::vector<Move> ChessBoard::generateAdvisorMoves(int x, int y) const
{
    assert(board[y][x].type == ChessPieceType::Advisor);
    ChessBoardPointState state = board[y][x].state;
    assert(state == ChessBoardPointState::OurSide || state == ChessBoardPointState::OtherSide);

    static const int deltaXsForAdvisor[] = { -1, -1, 1, 1 };
    static const int deltaYsForAdvisor[] = { -1, 1, 1, -1 };

    std::vector<Move> ret;

    // 确定移动范围
    int yMin, yMax;
    if (state == ChessBoardPointState::OurSide)
    {
        yMin = 7;
        yMax = 9;
    }
    else
    {
        yMin = 0;
        yMax = 2;
    }

    for (int dir = 0; dir < 4; dir++)
    {
        int x_i = x + deltaXsForAdvisor[dir];
        int y_i = y + deltaYsForAdvisor[dir];
        if (x_i >= 3 && x_i < 6 && y_i >= yMin && y_i <= yMax)
        {
            if (board[y_i][x_i].state != state)
            {
                ret.push_back({ x, y, x_i, y_i });
            }
        }
    }

    return ret;
}

std::vector<Move> ChessBoard::generateKingMoves(int x, int y) const
{
    assert(board[y][x].type == ChessPieceType::King);
    ChessBoardPointState state = board[y][x].state;
    assert(state == ChessBoardPointState::OurSide || state == ChessBoardPointState::OtherSide);

    std::vector<Move> ret;

    // 确定移动范围
    int yMin, yMax;
    if (state == ChessBoardPointState::OurSide)
    {
        yMin = 7;
        yMax = 9;
    }
    else
    {
        yMin = 0;
        yMax = 2;
    }

    for (int dir = 0; dir < 4; dir++)
    {
        int x_i = x + deltaXs[dir];
        int y_i = y + deltaYs[dir];
        if (x_i >= 3 && x_i < 6 && y_i >= yMin && y_i <= yMax)
        {
            if (board[y_i][x_i].state != state)
            {
                ret.push_back({ x, y, x_i, y_i });
            }
        }
    }

    // 额外检查对方的将在正对面且没有障碍的情况
    bool bOtherKingDirectlySeen = false;
    int yOtherKing = -1;

    int deltaY;
    if (state == ChessBoardPointState::OurSide)
    {
        deltaY = -1;
    }
    else
    {
        deltaY = 1;
    }
    for (int y_i = y + deltaY; y_i >= 0 && y_i < 10; y_i += deltaY)
    {
        if (board[y_i][x].state != ChessBoardPointState::Empty && board[y_i][x].type == ChessPieceType::King)
        {
            bOtherKingDirectlySeen = true;
            yOtherKing = y_i;
            break;
        }
        else if (board[y_i][x].state != ChessBoardPointState::Empty)
        {
            break;
        }
    }
    if (bOtherKingDirectlySeen)
    {
        ret.push_back({ x, y, x, yOtherKing });
    }

    return ret;
}

std::vector<Move> ChessBoard::generateCannonMoves(int x, int y) const
{
    assert(board[y][x].type == ChessPieceType::Cannon);
    ChessBoardPointState state = board[y][x].state;
    assert(state == ChessBoardPointState::OurSide || state == ChessBoardPointState::OtherSide);

    std::vector<Move> ret;

    for (int dir = 0; dir < 4; dir++)
    {
        // 首先需要找到炮枪口作为中继点，在找到中继点之前的任何一个位置都可以移动到那里
        int relayX = -1, relayY = -1;
        for (int x_i = x + deltaXs[dir], y_i = y + deltaYs[dir]; x_i >= 0 && x_i < 9 && y_i >= 0 && y_i < 10; x_i += deltaXs[dir], y_i += deltaYs[dir])
        {
            if (board[y_i][x_i].state == ChessBoardPointState::Empty)
            {
                ret.push_back({ x, y, x_i, y_i });
            }
            else
            {
                relayX = x_i;
                relayY = y_i;
                break;
            }
        }
        if (relayX == -1) continue;

        // 然后寻找目标
        int targetX = -1, targetY = -1;
        for (int x_i = relayX + deltaXs[dir], y_i = relayY + deltaYs[dir]; x_i >= 0 && x_i < 9 && y_i >= 0 && y_i < 10; x_i += deltaXs[dir], y_i += deltaYs[dir])
        {
            if (board[y_i][x_i].state != ChessBoardPointState::Empty)
            {
                targetX = x_i;
                targetY = y_i;
                break;
            }
        }
        if (targetX == -1) continue;
        if (board[targetY][targetX].state != state)
        {
            ret.push_back({ x, y, targetX, targetY });
        }
    }

    return ret;
}

std::vector<Move> ChessBoard::generatePawnMoves(int x, int y) const
{
    assert(board[y][x].type == ChessPieceType::Pawn);
    ChessBoardPointState state = board[y][x].state;
    assert(state == ChessBoardPointState::OurSide || state == ChessBoardPointState::OtherSide);

    std::vector<Move> ret;

    // 对应于上、左、右的x、y偏移值
    static const int deltaXForPawnForOurSide[] = { 0, -1, 1 };
    static const int deltaYForPawnForOurSide[] = { -1, 0, 0 };
    // 对应于下、左、右的x、y偏移值
    static const int deltaXForPawnForOtherSide[] = { 0, -1, 1 };
    static const int deltaYForPawnForOtherSide[] = { 1, 0, 0 };

    // 如果还未过河，只能向前走
    int dirMax;
    if (state == ChessBoardPointState::OurSide)
    {
        if (y < 5)
        {
            dirMax = 2;
        }
        else
        {
            dirMax = 0;
        }
    }
    else
    {
        if (y >= 5)
        {
            dirMax = 2;
        }
        else
        {
            dirMax = 0;
        }
    }

    const int* deltaXForPawn;
    const int* deltaYForPawn;
    if (state == ChessBoardPointState::OurSide)
    {
        deltaXForPawn = deltaXForPawnForOurSide;
        deltaYForPawn = deltaYForPawnForOurSide;
    }
    else
    {
        deltaXForPawn = deltaXForPawnForOtherSide;
        deltaYForPawn = deltaYForPawnForOtherSide;
    }

    for (int dir = 0; dir <= dirMax; dir++)
    {
        int x_i = x + deltaXForPawn[dir];
        int y_i = y + deltaYForPawn[dir];
        if (x_i >= 0 && x_i < 9 && y_i >= 0 && y_i < 10)
        {
            if (board[y_i][x_i].state != state)
            {
                ret.push_back({ x, y, x_i, y_i });
            }
        }
    }

    return ret;
}

std::vector<Move> ChessBoard::generateMovesForChessPiece(int x, int y) const
{
    ChessPieceType type = board[y][x].type;
    switch (type)
    {
    case ChessPieceType::Rook:
        return generateRookMoves(x, y);
    case ChessPieceType::kNight:
        return generateKnightMoves(x, y);
    case ChessPieceType::Bishop:
        return generateBishopMoves(x, y);
    case ChessPieceType::Advisor:
        return generateAdvisorMoves(x, y);
    case ChessPieceType::King:
        return generateKingMoves(x, y);
    case ChessPieceType::Cannon:
        return generateCannonMoves(x, y);
    case ChessPieceType::Pawn:
        return generatePawnMoves(x, y);
    default:
        qCritical() << "Invalid chess piece type";
        exit(2);
    }
}

int ChessBoard::evaluateChessPieceAdvantage(int x, int y) const
{
    ChessBoardPointState state = board[y][x].state;
    assert(state == ChessBoardPointState::OurSide || state == ChessBoardPointState::OtherSide);

    // 1. 棋力评估（根据棋子的位置）
    // 如果是对方的棋子，将坐标转换为对方视角
    int tX = x, tY = y;
    if (state == ChessBoardPointState::OtherSide)
    {
        transformCoordinates(tX, tY);
    }
    int positionEvaluation = ChessPiecePositionEvaluation(board[y][x].type, tX, tY);

    // 2. 行棋可能性评估（根据可以吃掉的敌方棋子）
    std::vector<Move> possibleMoves = generateMovesForChessPiece(x, y);
    std::vector<ChessPieceType> chessPiecesCanBeCaptured;
    for (const auto& move : possibleMoves)
    {
        if (board[move.nextY][move.nextX].state != ChessBoardPointState::Empty)
        {
            chessPiecesCanBeCaptured.push_back(board[move.nextY][move.nextX].type);
        }
    }
    int moveEvaluation = ChessPieceMoveEvaluation(chessPiecesCanBeCaptured);

    // 3. 棋子价值评估
    int valueEvaluation = ChessPieceValueEvaluation(board[y][x].type);

    return positionEvaluation + moveEvaluation + valueEvaluation;
}
