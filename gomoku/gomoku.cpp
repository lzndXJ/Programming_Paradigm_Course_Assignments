#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <chrono>

using namespace std;

//==========常量的定义==========

//一个位置上棋子的状态
const int WHITE = 0;
const int BLACK = 1;
const int EMPTY = 2;

//给7种棋型编号
const int FIVE = 7;
const int LIVEFOUR = 6;
const int RUSHFOUR = 5;
const int LIVETHREE = 4;
const int SLEEPTHREE = 3;
const int LIVETWO = 2;
const int SLEEPTWO = 1;
const int NONE = 0;

//定义长整型别名
typedef unsigned long long ULL;

//定义棋盘的相关常量
const int BOARDSIZE = 12;     //棋盘尺寸
const int TT_SIZE = 1 << 18;  //置换表大小

//计时功能
chrono::high_resolution_clock::time_point startTime;

//==========Board类==========

//坐标
struct Pos {
	int row;
	int col;
	Pos() : row(0), col(0) {}
	Pos(int rr, int cc) : row(rr), col(cc) {}
};

//单个点上的信息
struct PointInfo {
	int color;
	int chessPattern[2][4];//[黑白棋][四个方向上的棋型]
};

//存储各种棋型的棋谱
struct TemplateVariant {
	string pattern;
	vector<int> center_pos;   // 该模式中己方棋子的索引（可对齐位置）
};

static vector<vector<TemplateVariant>> patternTable;

//棋盘模块
class Board
{
public:
	int size;        //棋盘尺寸
	int chessCount;  //落子数
	int boardStart = 0, boardEnd = BOARDSIZE - 1;  //在数组中棋盘的真实边界

	PointInfo chessBoard[BOARDSIZE][BOARDSIZE];   //每个位置的信息
	ULL zobristBoard[BOARDSIZE][BOARDSIZE][2];    //存储每个位置的哈希值
	ULL currentHash;              //当前棋局键值，每一次落子都会更新键值

	//遍历棋盘的4个方向
	const int dr[4] = { 0, 1, 1, -1 };
	const int dc[4] = { 1, 0, 1, 1 };

	//初始化
	Board();              //构造函数
	~Board();
	ULL random_num();     //生成随机哈希值
	void init_zobrist_board();    //初始化键值表
	void init_pattern_table();    //初始化棋型模板

	//棋盘操作函数
	bool is_inside(int r, int c) const;    //判断是否在界内
	bool is_empty(int r, int c) const;    //判断是否是空的
	void place_stone(int r, int c, int color);  //在棋盘上下子并更新各个信息
	void remove_stone(int r, int c);            //悔棋，用于搜索算法

	//棋型分析与更新
	int get_pattern(int r, int c, int color, int dir);     //获取某个位置在某个方向上是否有棋型
	void update_point_info(int r, int c);                  //更新某个点及其周围的棋型

	//哈希相关操作
	void update_hash(int r, int c, int oldColor, int color);         // 落子或撤销时更新哈希值
	ULL get_hash() const;    // 获取当前局面哈希值

	//判断棋局是否结束
	bool is_win(int color);
	bool is_terminal();
};

//初始化
Board::Board() {
	size = BOARDSIZE;
	chessCount = 0;

	// 初始化棋盘所有位置为空
	for (int r = 0; r < BOARDSIZE; ++r) {
		for (int c = 0; c < BOARDSIZE; ++c) {
			chessBoard[r][c].color = EMPTY;
			memset(chessBoard[r][c].chessPattern, 0, sizeof(chessBoard[r][c].chessPattern));  // 初始化棋型为空
		}
	}

	init_zobrist_board();
	init_pattern_table();
}

Board::~Board() {}

ULL Board::random_num() {
	return rand() ^ ((ULL)rand() << 20) ^ ((ULL)rand() << 30) ^ ((ULL)rand() << 40);
}

void Board::init_zobrist_board() {
	srand((unsigned int)time(0));
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			zobristBoard[i][j][WHITE] = random_num();
			zobristBoard[i][j][BLACK] = random_num();
		}
	}

	currentHash = 0;
}

void Board::init_pattern_table() {
	patternTable.resize(8);  //八种棋型
	auto add_pattern = [&](int type, string pat) {
		TemplateVariant tv;
		tv.pattern = pat;
		for (int i = 0; i < (int)pat.size(); i++)
			if (pat[i] == '0')
				tv.center_pos.push_back(i);      // 记录己方棋子('0')在模式中的位置
		patternTable[type].push_back(tv);

		// 自动加入镜像模板
		string rev = pat;
		reverse(rev.begin(), rev.end());
		if (rev != pat) {
			TemplateVariant rv;
			rv.pattern = rev;
			for (int i = 0; i < (int)rev.size(); i++)
				if (rev[i] == '0')
					rv.center_pos.push_back(i);
			patternTable[type].push_back(rv);
		}
		};

	// 各种棋型模板（来自你给的定义）
	add_pattern(FIVE, "00000");

	add_pattern(LIVEFOUR, "200002");

	add_pattern(RUSHFOUR, "200001");
	add_pattern(RUSHFOUR, "02000");
	add_pattern(RUSHFOUR, "00200");

	add_pattern(LIVETHREE, "200022");
	add_pattern(LIVETHREE, "202002");

	add_pattern(SLEEPTHREE, "220001");
	add_pattern(SLEEPTHREE, "202001");
	add_pattern(SLEEPTHREE, "200201");
	add_pattern(SLEEPTHREE, "02200");
	add_pattern(SLEEPTHREE, "02020");
	add_pattern(SLEEPTHREE, "1200021");

	add_pattern(LIVETWO, "220022");
	add_pattern(LIVETWO, "20202");
	add_pattern(LIVETWO, "0220");

	add_pattern(SLEEPTWO, "222001");
	add_pattern(SLEEPTWO, "220201");
	add_pattern(SLEEPTWO, "202201");
	add_pattern(SLEEPTWO, "02220");
}

//操作棋盘函数
bool Board::is_inside(int r, int c) const {
	return r >= boardStart && r <= boardEnd && c >= boardStart && c <= boardEnd;
}

bool Board::is_empty(int r, int c) const {
	return chessBoard[r][c].color == EMPTY;
}

void Board::place_stone(int r, int c, int color) {
	if (!is_inside(r, c) || chessBoard[r][c].color != EMPTY)
		return;
	int oldColor = chessBoard[r][c].color;
	chessBoard[r][c].color = color;
	update_hash(r, c, oldColor, color);
	chessCount++;
	update_point_info(r, c);
}

void Board::remove_stone(int r, int c) {
	if (!is_inside(r, c) || is_empty(r, c))
		return;
	int oldColor = chessBoard[r][c].color;
	chessBoard[r][c].color = EMPTY;
	update_hash(r, c, oldColor, EMPTY);
	chessCount--;
	update_point_info(r, c);
}

//棋型分析函数
int Board::get_pattern(int r, int c, int color, int dir) {
	// 构造长度 9 的窗口 line（中心为 index=4）
	string line(9, '1');  // 默认1

	for (int i = -4; i <= 4; ++i) {
		int nr = r + dr[dir] * i;
		int nc = c + dc[dir] * i;
		int idx = i + 4;

		// 越界或不在有效棋盘区域 → 当成 '1'
		if (!is_inside(nr, nc)) {
			line[idx] = '1';
			continue;
		}

		int cell = chessBoard[nr][nc].color;

		if (cell == color)
			line[idx] = '0';        // 己方棋子
		else if (cell == EMPTY)
			line[idx] = '2';        // 空位
		else
			line[idx] = '1';        // 对方棋子 / OUTSIDE
	}

	// 依次判断：FIVE → ... → SLEEPTWO
	for (int type = FIVE; type >= SLEEPTWO; --type) {
		for (const auto& tv : patternTable[type]) {

			// 遍历所有匹配出现位置
			size_t pos = line.find(tv.pattern, 0);
			while (pos != string::npos) {

				// 检查模板中所有己方棋子是否可能对齐当前点
				if (pos <= 4 && 4 < pos + tv.pattern.size()) {
					return type;
				}
				// 查找下一个出现位置
				pos = line.find(tv.pattern, pos + 1);
			}
		}
	}
	return NONE;
}

void Board::update_point_info(int r, int c) {
	for (int dir = 0; dir < 4; dir++) { //下一颗棋，周围的棋型都会变
		for (int d = -4; d <= 4; d++) {
			int nr = r + dr[dir] * d;
			int nc = c + dc[dir] * d;

			// 必须只在 MAXSIZE 范围内更新，不是有效区范围
			if (!is_inside(nr, nc))
				continue;

			int color = chessBoard[nr][nc].color;

			if (color == WHITE) {
				chessBoard[nr][nc].chessPattern[WHITE][dir] = get_pattern(nr, nc, WHITE, dir);
			}
			else if (color == BLACK) {
				chessBoard[nr][nc].chessPattern[BLACK][dir] = get_pattern(nr, nc, BLACK, dir);
			}
		}
	}
}

//哈希相关操作
void Board::update_hash(int r, int c, int oldColor, int color) {
	if (oldColor != EMPTY)
		currentHash ^= zobristBoard[r][c][oldColor];                 // 移除旧状态的哈希
	if (color != EMPTY)
		currentHash ^= zobristBoard[r][c][color];                    // 添加新状态的哈希
}

ULL Board::get_hash()const {
	return currentHash;
}

//判断是否结束
bool Board::is_win(int color) {
	for (int r = boardStart; r <= boardEnd; ++r)
	{
		for (int c = boardStart; c <= boardEnd; ++c)
		{
			if (chessBoard[r][c].color != color) continue;

			// 如果任何方向上是 FIVE 就赢
			for (int dir = 0; dir < 4; dir++)
			{
				if (chessBoard[r][c].chessPattern[color][dir] == FIVE)
					return true;
			}
		}
	}
	return false;
}

bool Board::is_terminal() {
	return is_win(WHITE) || is_win(BLACK) || chessCount == BOARDSIZE * BOARDSIZE;
}

//==========Ai类==========

//一个候选的走法
struct Move {
	int r, c;   //下在哪里
	int score;  //走这一步的评分
};

//候选走法列标
struct MoveList {
	Move move[64];
	int size;       //当前数量
};
//AI模块
class Ai
{
public:
	Board* board;           //指向主棋盘
	int myColor;            // AI 扮演的颜色
	int oppColor;           // 对手颜色
	int maxDepth;           // 最大搜索深度
	int timeLimit;   // 每步思考时间（毫秒）
	bool stopThinking;      // 控制是否超时停止
	int nodeCount;          // 搜索节点计数

	Ai(Board* b);           //构造函数
	//根决策函数
	Pos make_decision(int color);  //选择最佳落子点

	//评估函数
	int pattern_value(int pat) const;    //每种棋型对应一个分数
	int evaluate(int color) const;             //对整个棋盘打分

	//启发式走法生成
	MoveList generate_moves(int color);  //走法生成，附近两格无子忽略，优先包含冲四活三

	//Alpha-Beta搜索
	int alpha_beta(int depth, int alpha, int beta, int color);

	//迭代加深搜索
	Pos iterative_deepening(int maxDepth, int color);  //用于控制思考时间，每一层向下加深

	//置换表
	struct TTEntry {
		ULL hash;       // 棋盘哈希值
		int depth;      // 搜索深度
		int score;      // 评估分数
		int flag;       // EXACT / LOWERBOUND / UPPERBOUND
		Pos bestMove;   // 最佳走法（用于搜索走法排序）
	};

	vector<TTEntry> transTable;

	bool tt_lookup(ULL hash, TTEntry& entry);  //查找置换表
	void tt_store(ULL hash, int depth, int score, int flag, const Pos& bestMove);   //存储搜索结果到置换表

	//辅助工具函数
	bool time_out() const;       // 检查是否超时
	bool valid_move(int r, int c) const; // 是否为合法落点
	bool is_near(int r, int c) const;    //判断该点周围两格内是否有棋子
	int move_score(int r, int c, int color) const;   //返回该点的启发式评分
};

//构造函数
Ai::Ai(Board* b)
	: board(b),
	myColor(BLACK),
	oppColor(WHITE),
	maxDepth(4),
	timeLimit(1900),
	stopThinking(false),
	nodeCount(0)
{
	transTable.resize(TT_SIZE);
}
//根决策函数
Pos Ai::make_decision(int color)
{
	startTime = chrono::high_resolution_clock::now();
	stopThinking = false;

	myColor = color;
	oppColor = (color == BLACK ? WHITE : BLACK);

	//一般情况，使用迭代加深

	stopThinking = false;
	nodeCount = 0;

	Pos best = iterative_deepening(maxDepth, myColor);

	return best;
}

//评估函数
int Ai::pattern_value(int pat)const {
	switch (pat)
	{
		case FIVE:        return 100000;
		case LIVEFOUR:    return 10000;
		case RUSHFOUR:    return 5000;
		case LIVETHREE:   return 1000;
		case SLEEPTHREE:  return 200;
		case LIVETWO:     return 50;
		case SLEEPTWO:    return 10;
		default:          return 0;
	}
}

int Ai::evaluate(int color) const {
	int myScore = 0;
	int oppScore = 0;
	int opp = (color == BLACK ? WHITE : BLACK);

	// 用于记录已统计过的棋型
	// 存储格式: (起始行, 起始列, 方向, 棋型类型)
	// 注意：不需要存储颜色，因为同一线上只有一种颜色的棋型
	static bool counted[BOARDSIZE][BOARDSIZE][4];
	memset(counted, 0, sizeof(counted));

	for (int r = board->boardStart; r <= board->boardEnd; r++) {
		for (int c = board->boardStart; c <= board->boardEnd; c++) {
			if (board->chessBoard[r][c].color == EMPTY)
				continue;

			int cellColor = board->chessBoard[r][c].color;

			for (int dir = 0; dir < 4; dir++) {
				int pat = board->chessBoard[r][c].chessPattern[cellColor][dir];
				if (pat == NONE)
					continue;

				// 找到这个棋型在这条线上的"最小坐标点"作为唯一标识
				int minR = r, minC = c;

				// 向后搜索，找到这条线上所有同色棋子的最小坐标
				for (int d = -4; d <= 4; d++) {
					int nr = r + board->dr[dir] * d;
					int nc = c + board->dc[dir] * d;

					if (!board->is_inside(nr, nc))
						continue;

					// 检查是否是同色棋子，且有相同的棋型
					if (board->chessBoard[nr][nc].color == cellColor &&
						board->chessBoard[nr][nc].chessPattern[cellColor][dir] == pat) {

						// 更新最小坐标（用于唯一标识这个棋型）
						if (nr < minR || (nr == minR && nc < minC)) {
							minR = nr;
							minC = nc;
						}
					}
				}

				//使用布尔数组查重
				if (counted[minR][minC][dir])
					continue;
				counted[minR][minC][dir] = true;

				// 统计分数
				int val = pattern_value(pat);
				if (cellColor == color)
					myScore += val;
				else
					oppScore += val;
			}
		}
	}

	return myScore - oppScore;
}

//启发式走法生成
MoveList Ai::generate_moves(int color) {
	MoveList ml;
	ml.size = 0;

	int opp = (color == BLACK ? WHITE : BLACK);

	vector<Move> mustWin, mustBlock, strongMoves, normalMoves;

	//预先扫描对手的全局最大威胁
	int globalOppMaxThreat = NONE;
	vector<Pos> oppThreatPoints;  // 记录对手威胁点位置

	for (int r = board->boardStart; r <= board->boardEnd; r++) {
		for (int c = board->boardStart; c <= board->boardEnd; c++) {
			if (board->chessBoard[r][c].color != opp)
				continue;

			// 检查对手这个棋子在4个方向的棋型
			for (int dir = 0; dir < 4; dir++) {
				int pat = board->chessBoard[r][c].chessPattern[opp][dir];
				if (pat > globalOppMaxThreat) {
					globalOppMaxThreat = pat;
					oppThreatPoints.clear();
					oppThreatPoints.push_back(Pos(r, c));
				}
				else if (pat == globalOppMaxThreat && pat >= LIVEFOUR) {
					// 记录同等级的高威胁点
					oppThreatPoints.push_back(Pos(r, c));
				}
			}
		}
	}

	// 扫描候选点
	for (int r = board->boardStart; r <= board->boardEnd; r++) {
		for (int c = board->boardStart; c <= board->boardEnd; c++) {
			if (!board->is_inside(r, c) || !board->is_empty(r, c))
				continue;
			if (!is_near(r, c)) continue;  // 两格内没有棋子，忽略

			//模拟我方落子
			board->place_stone(r, c, color);

			bool myWin = false;
			bool strongThreat = false;
			bool thinThreat = false;
			int myScore = 0;

			for (int dir = 0; dir < 4; dir++) {
				int pat = board->chessBoard[r][c].chessPattern[color][dir];
				if (pat == FIVE) myWin = true;
				if (pat == LIVEFOUR) strongThreat = true;
				if (pat == RUSHFOUR) thinThreat = true;
				myScore += pattern_value(pat);
			}

			board->remove_stone(r, c);

			// === 优化后的防守检查 ===
			int localOppThreat = NONE;

			// 检查这个位置能否阻挡对手的威胁
			if (globalOppMaxThreat >= RUSHFOUR) {
				// 只有对手有强威胁时才详细检查
				board->place_stone(r, c, color);  // 假设在这里防守

				// 检查是否能破坏对手的威胁
				bool canBlock = false;
				for (const auto& oppPos : oppThreatPoints) {
					bool stillThreat = false;
					for (int dir = 0; dir < 4; dir++) {
						int pat = board->chessBoard[oppPos.row][oppPos.col].chessPattern[opp][dir];
						if (pat >= RUSHFOUR) {
							stillThreat = true;
							break;
						}
					}
					if (!stillThreat) {
						canBlock = true;
						break;
					}
				}

				if (canBlock) {
					localOppThreat = globalOppMaxThreat;
				}

				board->remove_stone(r, c);
			}

			//检查这个位置附近的对手威胁（局部检查）
			if (localOppThreat < RUSHFOUR) {
				// 只检查邻近的对手棋子
				for (int dr = -1; dr <= 1; dr++) {
					for (int dc = -1; dc <= 1; dc++) {
						if (dr == 0 && dc == 0) continue;
						int nr = r + dr;
						int nc = c + dc;
						if (!board->is_inside(nr, nc))
							continue;
						if (board->chessBoard[nr][nc].color != opp)
							continue;

						// 检查这个对手棋子的威胁等级
						for (int dir = 0; dir < 4; dir++) {
							int pat = board->chessBoard[nr][nc].chessPattern[opp][dir];
							if (pat > localOppThreat) {
								localOppThreat = pat;
							}
						}
					}
				}
			}

			//分类候选走法
			if (myWin) {
				mustWin.push_back(Move{ r, c, 100000 });
			}
			else if (globalOppMaxThreat >= RUSHFOUR) {
				// 对手有活四或五连威胁，必须防守
				if (localOppThreat >= RUSHFOUR) {
					mustBlock.push_back(Move{ r, c, 90000 + myScore });
				}
			}
			else if (strongThreat)
				strongMoves.push_back(Move{ r, c, myScore + 70000 });
			else if (globalOppMaxThreat == LIVETHREE) {
				// 对手有活三，优先但不是唯一选择
				if (localOppThreat == LIVETHREE || strongThreat) {
					strongMoves.push_back(Move{ r, c, myScore + 10000 });
				}
				else {
					normalMoves.push_back(Move{ r, c, myScore });
				}
			}
			else if (thinThreat) {
				strongMoves.push_back(Move{ r, c, myScore + 5000 });
			}
			else {
				normalMoves.push_back(Move{ r, c, myScore });
			}
		}
	}

	//合并候选走法
	auto append = [&](vector<Move>& v) {
		for (auto& m : v) {
			if (ml.size < 64)  // 增加容量限制
				ml.move[ml.size++] = m;
		}
		};

	append(mustWin);
	append(mustBlock);
	append(strongMoves);
	append(normalMoves);

	// 排序
	sort(ml.move, ml.move + ml.size, [](const Move& a, const Move& b) {
		return a.score > b.score;
		});

	// 限制候选数量
	if (ml.size > 20) ml.size = 20;

	return ml;
}

//Alpha-Beta搜索
int Ai::alpha_beta(int depth, int alpha, int beta, int color) {
	//基础设定
	const int INF = 1000000000;       //无穷大
	const int WIN_SCORE = 10000000;   // 代表绝杀局面
	nodeCount++;                      // 搜索节点统计（性能分析）

	int origAlpha = alpha;            // 储存原始 alpha 用于 later flag 判断（非常重要）
	int origBeta = beta;

	//终局条件判断
	//这里必须返回“剩余深度越深，胜利越快”的评分。
	//用 WIN_SCORE - (maxDepth - depth) 表示快赢更好。
	if (board->is_win(color))
		return WIN_SCORE - (maxDepth - depth);  //depth是剩余可搜索的深度

	// 对手赢了 → 负分，越早输越难看
	if (board->is_win(color == BLACK ? WHITE : BLACK))
		return -(WIN_SCORE - (maxDepth - depth));

	// 搜索到最底层 → 直接用 evaluate()
	if (depth <= 0)
		return evaluate(color);

	//如果以前搜过，而且搜索深度 ≥ 当前需要的深度，就可以直接拿来用。
	ULL hash = board->get_hash();
	TTEntry entry;

	if (tt_lookup(hash, entry)) {    // 找到了
		if (entry.depth >= depth) {  // 搜过更深的？

			if (entry.flag == 0) {
				// EXACT：精确值，可直接返回
				return entry.score;
			}
			else if (entry.flag == 1) {
				// LOWERBOUND：表示真实 score ≥ entry.score
				// → 用它提升 alpha
				alpha = max(alpha, entry.score);
			}
			else if (entry.flag == 2) {
				// UPPERBOUND：表示真实 score ≤ entry.score
				beta = min(beta, entry.score);
			}

			// alpha ≥ beta，发生剪枝，直接返回置换表结果
			if (alpha >= beta)
				return entry.score;
		}
	}
	//搜索候选走法（走法生成 + 启发式排序）
	MoveList ml = generate_moves(color);
	if (ml.size == 0)
		return 0;   // 没有走法的话，平局 or 死局

	//AlphaBeta Negamax 递归主体
	int bestScore = -INF;     // 当前节点能取得的最好得分
	Pos bestMove = Pos();     // 最佳走法，用于置换表存储

	int nextColor = (color == BLACK ? WHITE : BLACK);

	for (int i = 0; i < ml.size; ++i) {

		//超时检测
		if (time_out()) {
			stopThinking = true;
			break;
		}

		int r = ml.move[i].r;
		int c = ml.move[i].c;

		//尝试走子
		board->place_stone(r, c, color);

		int score;

		// 若这一步直接获胜，优先级极高
		if (board->is_win(color)) {
			score = WIN_SCORE - (maxDepth - depth);
		}
		else {
			//Negamax 核心逻辑
			// alpha_beta(depth - 1, -beta, -alpha, nextColor)
			//自己最大化 = 对手最小化，所以分数取负号，搜索窗口也取负号
			score = -alpha_beta(depth - 1, -beta, -alpha, nextColor);
		}

		//撤销走子
		board->remove_stone(r, c);

		if (stopThinking)
			break;

		//更新最佳值 
		if (score > bestScore) {
			bestScore = score;
			bestMove = Pos(r, c);
		}

		//Alpha 剪枝更新 
		alpha = max(alpha, score);
		if (alpha >= beta) {  //我方保证的最高分大于对方保证的最低分，对方肯定不会选，直接剪掉
			break;
		}
	}
	//写入置换表
	int flag;
	if (bestScore <= origAlpha)        flag = 2; // UPPERBOUND
	else if (bestScore >= origBeta)   flag = 1; // LOWERBOUND
	else                              flag = 0; // EXACT
	tt_store(hash, depth, bestScore, flag, bestMove);

	return bestScore;
}

//迭代加深搜索
Pos Ai::iterative_deepening(int maxD, int color)
{
	// 初始化
	startTime = chrono::high_resolution_clock::now();
	stopThinking = false;
	nodeCount = 0;

	myColor = color;
	oppColor = (color == BLACK ? WHITE : BLACK);

	Pos finalBest = Pos(board->boardStart + board->size / 2,
		board->boardStart + board->size / 2);  // 默认下中间

	int finalScore = -1000000000;

	// 逐步加深：1层到maxD 层
	for (int depth = 1; depth <= maxD; depth++) {

		int alpha = -1000000000;
		int beta = 1000000000;

		// 当前深度的搜索结果
		int score = alpha_beta(depth, alpha, beta, myColor);

		if (stopThinking) {
			// 超时,不能使用当前层结果，因为可能没搜索完
			break;
		}

		// 记录当前深度的结果
		finalScore = score;

		// 搜索成功后，从置换表取出该层的最佳走法
		ULL hash = board->get_hash();
		TTEntry entry;
		if (tt_lookup(hash, entry)) {
			finalBest = entry.bestMove;
		}

		// 记录当前成功的深度（用于 AlphaBeta 的 WIN_SCORE 修正）
		maxDepth = depth;
	}

	return finalBest;
}

//置换表
bool Ai::tt_lookup(ULL hash, TTEntry& entry)
{
	int idx = hash % TT_SIZE;
	TTEntry& e = transTable[idx];

	if (e.hash == hash && e.depth > 0) {
		entry = e;
		return true;
	}
	return false;
}

void Ai::tt_store(ULL hash, int depth, int score, int flag, const Pos& bestMove)
{
	int idx = hash % TT_SIZE;
	TTEntry& e = transTable[idx];

	if (e.depth <= depth || e.hash != hash) {
		e.hash = hash;
		e.depth = depth;
		e.score = score;
		e.flag = flag;
		e.bestMove = bestMove;
	}
}

//辅助工具函数
bool Ai::time_out() const {
	if (stopThinking) return true;

	auto now = chrono::high_resolution_clock::now();
	long long elapsed_ms =
		chrono::duration_cast<chrono::milliseconds>(now - startTime).count();

	return elapsed_ms >= timeLimit;
}

bool Ai::valid_move(int r, int c) const {
	return board->is_inside(r, c) && board->is_empty(r, c);
}

bool Ai::is_near(int r, int c) const {
	// 开局时使用更严格的距离限制
	int maxDist = (board->chessCount < 10) ? 1 : 2;

	for (int dr = -maxDist; dr <= maxDist; dr++) {
		for (int dc = -maxDist; dc <= maxDist; dc++) {
			if (dr == 0 && dc == 0) continue;
			int nr = r + dr, nc = c + dc;
			if (!board->is_inside(nr, nc)) continue;
			if (board->chessBoard[nr][nc].color != EMPTY)
				return true;
		}
	}
	return false;
}

int Ai::move_score(int r, int c, int color) const {
	int opp = (color == BLACK ? WHITE : BLACK);
	int score = 0;

	for (int dir = 0; dir < 4; dir++) {
		int pat_my = board->chessBoard[r][c].chessPattern[color][dir];
		int pat_opp = board->chessBoard[r][c].chessPattern[opp][dir];

		// 己方棋型更重要，给更高权重；对手威胁次之
		score += pattern_value(pat_my) * 10;
		score += pattern_value(pat_opp) * 8;
	}
	return score;
}


int main()
{
	string cmd;
	Board board;
	Ai ai(&board);
	while (cin >> cmd) {
		if (cmd == "START") {
			int color;
			cin >> color;
			ai.myColor = (color == 1 ? BLACK : WHITE);
			ai.oppColor = (color == 1 ? WHITE : BLACK);
			int mid = (board.boardStart + board.boardEnd) / 2;

			board.place_stone(mid, mid, WHITE);
			board.place_stone(mid, mid + 1, BLACK);
			board.place_stone(mid + 1, mid, BLACK);
			board.place_stone(mid + 1, mid + 1, WHITE);
			cout << "OK\n" << flush;
		}

		else if (cmd == "PLACE") {
			int x, y;
			cin >> x >> y;
			board.place_stone(x, y, ai.oppColor);
		}

		else if (cmd == "TURN") {
			Pos best = ai.make_decision(ai.myColor);
			board.place_stone(best.row, best.col, ai.myColor);
			cout << best.row << " " << best.col << "\n" << flush;
		}
		else if (cmd == "END") {
			break;
		}
	}

	return 0;
}



