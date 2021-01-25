#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <queue>
#define MAP_SIZE 22
#define R first
#define C second
using namespace std;
typedef pair<int, int> COORD;
int dr[4] = { -1, 0, 1, 0 };
int dc[4] = { 0, 1, 0, -1 };
typedef struct Fish {//물고기(상어) 정보
	int size;//크기
	COORD coord;//좌표
};
int map[MAP_SIZE][MAP_SIZE];
int N;	//공간 크기
vector<Fish> fish_list;	//상어가 먹을 물고기 정보 저장
Fish shark;	//상어
int time = 0; //문제의 답, 물고기 먹는데 걸리는 시간

bool compare1(Fish f1, Fish f2) {//물고기를 크기가 작은 순으로 정렬
	return f1.size < f2.size;
}

//void PrintVec(vector<int> v) {
//	for (int i = 0; i < v.size(); i++) {
//		cout << v[i] << " ";
//	}
//	cout << "\n";
//}

int CalcFishPath(COORD dest) {//dest :: 목적지
	queue<COORD> q; int dist[MAP_SIZE][MAP_SIZE];
	for (int i = 0; i < N; i++) {//dist-> -1로 초기화
		fill(dist[i], dist[i] + N, -1);
	}
	q.push(shark.coord); dist[shark.coord.R][shark.coord.C] = 0;	//상어 위치 거리 초기화
	while (!q.empty()) {
		COORD cur = q.front(); q.pop();
		if (cur == dest) {
			break;
		}
		for (int dir = 0; dir < 4; dir++) {
			int nr = cur.R + dr[dir]; int nc = cur.C + dc[dir];
			if (nr >= N || nr < 0 || nc >= N || nc < 0) continue;
			if ((map[nr][nc] >= 1 && map[nr][nc] <= 6) && 
				map[nr][nc] > shark.size) {//상어보다 크기가 큰 물고기는 못 지나감
				continue;
			}
			if (dist[nr][nc] >= 0) continue;	//0이상의 값을 가지면 이미 한번 거쳤다는 의미
			dist[nr][nc] = dist[cur.R][cur.C] + 1;
			q.push(make_pair(nr, nc));
		}
	}
	return dist[dest.R][dest.C];
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);

	//입력
	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
			if (map[i][j] == 9) {//상어
				Fish temp = {2, make_pair(i, j)};
				shark = temp;
			}
			else if (map[i][j] >= 1 && map[i][j] <= 6) {
				Fish temp = { map[i][j], make_pair(i, j) };
				fish_list.push_back(temp);
			}
		}
	}
	if (fish_list.empty()) {//물고기가 애초에 없음
		cout << time;//0찍고 끝
		return 0;
	}
	//fish_list를 크기로 오름차순 정렬
	sort(fish_list.begin(), fish_list.end(), compare1);

	int eat_num = 0;//상어가 물고기 먹은 개수
	while (!fish_list.empty()) {//모든 물고기를 다 먹을 수 있을 때까지
		vector<int> eat_list;//fish_list의 인자값을 저장
		for (int i = 0; i < fish_list.size(); i++) {
			//상어 사이즈보다 작은 물고기들을 모아본다.
			if (fish_list[i].size < shark.size) {
				eat_list.push_back(i);
			}
		}
		////test
		//for (int i = 0; i < fish_list.size(); i++) {
		//	cout << fish_list[i].size << " ";
		//}
		//cout << "\n";
		//cout << "eat_list :: "; PrintVec(eat_list);

		if (eat_list.empty()) {//크기 상으로 더이상 먹을 물고기가 없음
			break;
		}
		//bfs를 돌아서 각 물고기를 먹는데 까지 걸리는 거리 측정
		//select_fisth_seq :: 먹을 것으로 선택한 물고기, min_path :: 해당 물고기 먹는데 걸리는 거리(=시간)
		int select_fish_seq; int min_path = -1;
		bool can_eat = false;
		for (int i = 0; i < eat_list.size(); i++) {
			int fish_seq = eat_list[i];
			//bfs
			int path = CalcFishPath(fish_list[fish_seq].coord);
			////test 
			//cout << eat_list[i] << " :: " << path <<"\n";
			if (path >= 0) {//도달할 수 있음
				can_eat = true;
				if (min_path == -1 || min_path > path) {
					select_fish_seq = fish_seq; min_path = path;
				}
				else if (min_path == path) {
					if (fish_list[select_fish_seq].coord.R > fish_list[fish_seq].coord.R) {
						select_fish_seq = fish_seq; min_path = path;
					}
					else if (fish_list[select_fish_seq].coord.R == fish_list[fish_seq].coord.R) {
						if (fish_list[select_fish_seq].coord.C > fish_list[fish_seq].coord.C) {
							select_fish_seq = fish_seq; min_path = path;
						}
					}
				}
			}
		}
		if (!can_eat) {//먹을 수 있는 물고기까지 도달할 수 없음
			break;
		}
		//계산한 최소 거리, 그에 해당하는 물고기 번호로 정보 갱신
		//time 갱신
		time += min_path;
		//상어 위치 갱신
		shark.coord = fish_list[select_fish_seq].coord;
		//물고기 먹은 개수 갱신
		eat_num++;
		if (eat_num == shark.size) {//물고기 먹은 개수가 상어 사이즈 도달
			shark.size++;
			eat_num = 0; //초기화
		}
		//지도 갱신
		map[fish_list[select_fish_seq].coord.R][fish_list[select_fish_seq].coord.C] = 0;//빈칸으로 만들기 
		//물고기 리스트 갱신(먹은 물고기 삭제)
		vector<Fish>::iterator it = fish_list.begin() + select_fish_seq;
		fish_list.erase(it, it + 1);
	}
	cout << time;

	return 0;
}
