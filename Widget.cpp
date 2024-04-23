#include "Widget.h"
#include <QMouseEvent>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QThread>
#include "Policy.h"
#include "Worker.h"


Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::WidgetClass()), chessBoard{ "://chessboard/chessboard.txt" }
{
    ui->setupUi(this);
    setWindowTitle("中国象棋");
    setWindowIcon(QIcon(":/images/king_red.png"));

    // 禁止改变窗口大小
    setFixedSize(800, 800);

    // 隐藏一些提示文字
    ui->redCheckLabel->hide();
    ui->blackCheckLabel->hide();
    ui->gameOverLabel->hide();
    ui->thinkingLabel->hide();

    // 初始化选择框；不要在冒号后直接初始化，会产生bug
    selectionBox = new QLabel{ this };
    // selectionBox->setParent(this);
    selectionBox->setPixmap(QPixmap(":/images/selection_box.png"));
    selectionBox->setBaseSize(69, 70);
    selectionBox->hide();

    addChessPieces();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent* event)
{
    if (!bCanMove) return;
    if (event->button() != Qt::LeftButton) return;

    int x, y;
    bool inChessBoard = transformPosToChessBoardPos(event->x(), event->y(), x, y);

    // 如果选中的是棋盘外或者同一个棋子，那么取消选择
    if (!inChessBoard || (inChessBoard && x == selectedX && y == selectedY))
    {
        selectedX = -1;
        selectedY = -1;
        selectionBox->hide();
    }
    // 如果选中的是己方不同的棋子，那么选择新的棋子
    else if (chessBoard.getChessBoardPoint(x, y).state == ChessBoardPointState::OurSide)
    {
        selectedX = x;
        selectedY = y;
        selectionBox->show();
        selectionBox->move(getSelectionBoxPos(x, y));

        qDebug() << "selected: " << x << " " << y;
        qDebug() << int(chessBoard.getChessBoardPoint(x, y).state) << ' ' << int(chessBoard.getChessBoardPoint(x, y).type);
    }
    // 如果选择的是空白处或者敌方棋子，尝试移动棋子
    else
    {
        if (selectedX == -1)
        {
            QMessageBox::information(this, "提示", "请先选择一个己方棋子");
            return;
        }

        Move move{ selectedX, selectedY, x, y };
        if (!chessBoard.isValidMove(move))
        {
            QMessageBox::information(this, "提示", "无效的行动");
            // 同时取消选择
            selectedX = -1;
            selectedY = -1;
            selectionBox->hide();
            return;
        }

        // 成功移动棋子

        // 隐藏选择框并暂时禁止玩家操作
        selectedX = -1;
        selectedY = -1;
        selectionBox->hide();
        bCanMove = false;

        // 采取行动
        moveChessPiece(move);

        // 如果游戏已经结束，直接返回
        if (checkCheckAndGameOver()) return;

        // 对方开始思考
        QTimer::singleShot(1, this, &Widget::startThinking);

        /*
        // debug: 跳过对方行动
        chessBoard = chessBoard.skipCurrentTurn();
        bCanMove = true;
        */
    }
}

void Widget::startThinking()
{
    ui->thinkingLabel->show();

    // 在新的线程中开始思考
    // Create a thread
    QThread* thread = new QThread();
    Worker* worker = new Worker(chessBoard, bestMove);

    // Move the worker to the thread
    worker->moveToThread(thread);

    // Connect signals and slots
    QObject::connect(thread, &QThread::started, worker, &Worker::process);
    QObject::connect(worker, &Worker::finished, thread, &QThread::quit);
    QObject::connect(worker, &Worker::finished, worker, &QObject::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    QObject::connect(worker, &Worker::finished, this, &Widget::endThinking);

    // Start the thread
    thread->start();
}

void Widget::endThinking()
{
    ui->thinkingLabel->hide();
    moveChessPiece(bestMove);

    // 如果游戏已经结束，不再继续
    if (checkCheckAndGameOver()) return;

    bCanMove = true;
}

void Widget::addChessPieces()
{
    ChessBoardPoint point;
    for (int y{}; y < 10; y++)
    {
        for (int x{}; x < 9; x++)
        {
            point = chessBoard.getChessBoardPoint(x, y);
            if (point.state != ChessBoardPointState::Empty)
            {
                QLabel* pieceLabel;
                if (point.state == ChessBoardPointState::OurSide)
                {
                    switch (point.type)
                    {
                    case ChessPieceType::Rook:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/rook_red.png"));
                        break;
                    case ChessPieceType::kNight:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/knight_red.png"));
                        break;
                    case ChessPieceType::Bishop:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/bishop_red.png"));
                        break;
                    case ChessPieceType::Advisor:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/advisor_red.png"));
                        break;
                    case ChessPieceType::King:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/king_red.png"));
                        break;
                    case ChessPieceType::Cannon:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/cannon_red.png"));
                        break;
                    case ChessPieceType::Pawn:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/pawn_red.png"));
                        break;
                    }
                }
                else
                {
                    switch (point.type)
                    {
                    case ChessPieceType::Rook:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/rook_black.png"));
                        break;
                    case ChessPieceType::kNight:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/knight_black.png"));
                        break;
                    case ChessPieceType::Bishop:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/bishop_black.png"));
                        break;
                    case ChessPieceType::Advisor:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/advisor_black.png"));
                        break;
                    case ChessPieceType::King:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/king_black.png"));
                        break;
                    case ChessPieceType::Cannon:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/cannon_black.png"));
                        break;
                    case ChessPieceType::Pawn:
                        pieceLabel = new QLabel(this);
                        pieceLabel->setPixmap(QPixmap(":/images/pawn_black.png"));
                        break;
                    }
                }
                pieceLabel->setBaseSize(63, 63);
                pieceLabel->move(getChessPiecePos(x, y));
                chessPieces.insert(Coordinate{ x,y }, pieceLabel);
            }
        }
    }
}

QPoint Widget::getChessPiecePos(int x, int y) const
{
    return QPoint(int(chessBoardLeftUpX + squareWidth * x - 30), int(chessBoadLeftUpY + squareHeight * y - 35));
}

QPoint Widget::getSelectionBoxPos(int x, int y) const
{
    return QPoint(int(chessBoardLeftUpX + squareWidth * x - 35), int(chessBoadLeftUpY + squareHeight * y - 35));
}

void Widget::moveChessPiece(const Move& move)
{
    Coordinate initPoint{ move.initX, move.initY };
    Coordinate nextPoint{ move.nextX, move.nextY };

    // 如果吃掉敌方棋子，那么删除对应棋子的QLabel
    ChessBoardPoint nextChessBoardPoint = chessBoard.getChessBoardPoint(move.nextX, move.nextY);
    if (nextChessBoardPoint.state != ChessBoardPointState::Empty)
    {
        QLabel* label = chessPieces[nextPoint];
        delete label;
        chessPieces.remove(nextPoint);
    }
    // 移动棋子的QLabel
    QLabel* label = chessPieces[initPoint];
    label->move(getChessPiecePos(move.nextX, move.nextY));
    // 同时需要改变chessPieces中这个棋子的key
    chessPieces.remove(initPoint);
    chessPieces.insert(nextPoint, label);

    // 更新chessBoard
    chessBoard = chessBoard.takeAction(move);
}

int nearestInt(float x)
{
    return int(x + 0.5);
}

bool Widget::transformPosToChessBoardPos(int posX, int posY, int& x, int& y) const
{
    x = nearestInt((posX - chessBoardLeftUpX) / squareWidth);
    y = nearestInt((posY - chessBoadLeftUpY) / squareHeight);

    if (x < 0 || x >= 9 || y < 0 || y >= 10) return false;
    else return true;
}

bool Widget::checkCheckAndGameOver()
{
    bool bGameOver;
    // ChessBoard是通过“将”是否还存在来判断游戏是否结束的；但如果一方被将死，游戏也会立即结束
    bGameOver = chessBoard.isGameOver();

    if (!bGameOver)
    {
        // 是否被将军
        if (chessBoard.isCheck())
        {
            // 是否被将死
            if (isCheckmate(chessBoard))
            {
                bGameOver = true;
            }
            else
            {
                // 显示将军提示
                if (chessBoard.isOurTurn())
                {
                    ui->blackCheckLabel->show();
                    // 将这个label放到最上层
                    ui->blackCheckLabel->raise();
                    QTimer::singleShot(1000, [this]() { ui->blackCheckLabel->hide(); });
                }
                else
                {
                    ui->redCheckLabel->show();
                    ui->redCheckLabel->raise();
                    QTimer::singleShot(1000, [this]() { ui->redCheckLabel->hide(); });
                }
            }
        }
    }

    if (bGameOver)
    {
        // 游戏结束
        if (!chessBoard.isOurTurn())
        {
            ui->gameOverLabel->setText("红棋获胜");
            ui->gameOverLabel->raise();
            // 将字体颜色设为红色
            ui->gameOverLabel->setStyleSheet("color: red");
            ui->gameOverLabel->show();
        }
        else
        {
            ui->gameOverLabel->setText("黑棋获胜");
            ui->gameOverLabel->raise();
            ui->gameOverLabel->show();
        }
    }

    return bGameOver;
}
