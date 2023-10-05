/*
  team:接地
  author:土橋晴人

  main.cpp
*/

/*
  移動タイプ
  滞在:0
  移動:1
  建築:2
  解体:3

  方向
  無方向:0
  左上:1
  上:2
  右上:3
  右:4
  右下:5
  下:6
  左下:7
  左:8
*/

#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;

int people;//職人の人数
int turn_max = 0;//maxターン数
int turn = -1;//現在ターン
int turnseconds;//1ターンの秒数
int width,height;//縦と横の大きさ
vector<vector<int>> board(30,vector<int> (30));//フィールドの情報
vector<vector<int>> walls(30,vector<int> (30));//壁の位置
vector<vector<int>> territories(30,vector<int> (30));//領地の状況
vector<vector<int>> masons(30,vector<int> (30));//職人位置一時保管用
vector<pair<int,int>> mason(20);//職人の座標({x,y})
vector<pair<int,int>> vsmason(20);//相手職人の座標({x,y})
vector<tuple<int,int,int,bool,bool>> mason_info(20,{-1,-1,-1,false,false});//職人の情報を保管
//tuple(0:行動中のパターン1:現在行動ステップ 2:終了ステップ 3:行動中表示 4:障害物回避行動中表示)
bool first;//先攻後攻
vector<vector<int>> score(10,vector<int> (30));//評価点格納(人数*17パターンしかないね)
vector<pair<int,int>> result_move(10);//確定移動パターン
vector<stack<pair<int,int>>> emagency_move(10);//回避行動保持用

vector<pair<int,int>> move_pat{{0,0},{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{0,0}};

vector<vector<pair<int,int>>> move_cource{{{2,8},{2,2},{1,4},{2,2},{2,4},{1,5},{2,4},{2,6},{1,7},{2,6},{2,8},{1,1},{2,8}},//1,(縦5横5)13ターン,陣地6城壁9
                                          {{2,8},{2,2},{1,4},{2,2},{2,4},{1,6},{2,4},{2,6},{1,7},{2,6},{2,8},{1,1},{2,8}},//2,(1の左右反転)13ターン,陣地6城壁9
                                          {{2,8},{2,2},{2,4},{1,5},{2,4},{2,6},{1,7},{2,6},{2,8},{1,1},{2,8}},//3,(縦5横5)11ターン,陣地5城壁8
                                          {{2,8},{2,2},{1,4},{2,2},{2,4},{1,6},{2,4},{1,6},{2,4},{2,6},{2,8},{1,1},{2,8}},//4,(縦5横4)13ターン,陣地5城壁9
                                          {{2,8},{2,2},{1,4},{2,2},{2,4},{1,6},{2,4},{2,6},{2,8}},//5,(縦4横4)9ターン,陣地3城壁7         移動:1 建築:2
                                          {{2,8},{2,2},{2,4},{2,6}}};//6,(縦3横3)4ターン,陣地1城壁4(動けないとき専用)

/*vector<vector<pair<int,int>>> hosei{{{0,0},{0,0},{0,1},{0,1},{0,1},{1,2},{1,2},{1,2},{2,1},{2,1},{2,1},{1,0},{1,0}},//1,(縦5横5)13ターン,陣地6城壁9
                                    {{0,1},{0,1},{0,2},{0,2},{0,2},{1,2},{1,2},{1,2},{2,1},{2,1},{2,1},{1,0},{1,0}},//2,(1の左右反転)13ターン,陣地6城壁9
                                    {{2,8},{2,2},{2,4},{1,5},{2,4},{2,6},{1,7},{2,6},{2,8},{1,1},{2,8}},//3,(縦5横5)11ターン,陣地5城壁8
                                    {{2,8},{2,2},{1,4},{2,2},{2,4},{1,6},{2,4},{1,6},{2,4},{2,6},{2,8},{1,1},{2,8}},//4,(縦5横4)13ターン,陣地5城壁9
                                    {{2,8},{2,2},{1,4},{2,2},{2,4},{1,6},{2,4},{2,6},{2,8}}};//5,(縦4横4)9ターン,陣地3城壁7         移動:1 建築:2*/

int kaiseki(){
  ifstream ifs("./hozon.json");
  string jsonstr;

  if(ifs.fail()){//エラー分岐  これが出たら死
    cout << "File Open Error" << endl;
    return -1;
  }

  
  while(getline(ifs, jsonstr)){
    cout<<"[Read result] "<< endl;
  }
  

  auto jobj = nlohmann::json::parse(jsonstr);


  turn_max = jobj["matches"].at(0)["turns"];
  turnseconds = jobj["matches"].at(0)["turnSeconds"];
  people = jobj["matches"].at(0)["board"]["mason"];
  width = jobj["matches"].at(0)["board"]["width"];
  height = jobj["matches"].at(0)["board"]["height"];

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      board[i][j] = jobj["matches"].at(0)["board"]["structures"].at(i).at(j);
    }
  }

  first = jobj["matches"].at(0)["first"];

  


  /*(デバッグ用)
  cout << "max_turn:" << turn_max << endl;
  cout << "turnsecond:" << turnseconds << endl;
  cout << "people:" << people << endl;
  cout <<  "width:" << width << endl;
  cout << "height:" << height << endl;
  
  cout << "board" << endl;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      cout << board[i][j] <<  " ";
    }
    cout << endl;
  }

  cout << "first?" << first << endl;
  cout << endl;*/
  

  return 1;
}

int kaiseki_loop(){
  ifstream ifs("./hozon_loop.json");
  string jsonstr;

  if(ifs.fail()){//エラー分岐  これが出たら死
    cout << "File Open Error" << endl;
    return -1;
  }

  
  while(getline(ifs, jsonstr)){
    //cout<<"[Read result] "<< endl;
  }

  if(jsonstr.empty()){
    return -1;
  }

  if(jsonstr == "TooEarly"){
    return 0;
  }
  

  auto jobj = nlohmann::json::parse(jsonstr);




  turn = jobj["turn"];

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      walls[i][j] = jobj["board"]["walls"].at(i).at(j);
    }
  }

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      territories[i][j] = jobj["board"]["territories"].at(i).at(j);
    }
  }

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      masons[i][j] = jobj["board"]["masons"].at(i).at(j);
    }
  }  

  //cout << "turn:" << turn << endl;


  /*(デバッグ用)
  cout << "turn:" << turn << endl;

  cout << "walls"<< endl;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      cout << walls[i][j] <<  " ";
    }
    cout << endl;
  }

  cout << endl;
  
  cout << "territories" << endl;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      cout << territories[i][j] <<  " ";
    }
    cout << endl;
  }

  cout << endl;

  cout << "mason" << endl;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      cout << masons[i][j] <<  " ";
    }
    cout << endl;
  }*/

  return 1;
}



void tokutei(){
  for(int k = 1; k <= people; k++){
    bool flag = false;
    for(int i = 0; i < height; i++){
      for(int j = 0; j < width; j++){
        if(masons[i][j] == k){
          mason[k] = {i,j};
          flag = true;
        }
        if(flag){
          break;
        }
      }
      if(flag){
        break;
      }
    }
  }

  for(int k = 1; k <= people; k++){
    bool flag = false;
    for(int i = 0; i < height; i++){
      for(int j = 0; j < width; j++){
        if(masons[i][j] == -1*k){
          vsmason[k] = {i,j};
          continue;
        }
        if(flag){
          break;
        }
      }
      if(flag){
        break;
      }
    }
  }
}

void move(){
  for(int p = 1; p <= people; p++){
		if(get<4>(mason_info[p])){
			hyouka(p,3);
		}
    if(get<3>(mason_info[p])){
      hyouka(p,2);
      
    }
    for(int i = 0; i < 2; i++){
      if(i == 0){
        bool flag_move = false;
        if(mason[p].first >= 1 & mason[p].second >= 1){
          if(mason[p].second < height-1 & mason[p].second < width-1){
            hyouka(p,1);
          }
          else{
            flag_move = true;
          }
        }
        else{
          flag_move = true;
        }

        if(flag_move){
          for(int j = 0; j < move_cource.size(); j++){
            score[p][j] = 0;
          }
        }
      }
      else if(i == 1){
        hyouka(p,4);
      }
    }
  }
}


void hyouka(int n, int m){
  if(m == 1){
    for(int i = 0; i < move_cource.size(); i++){
      int zahyou_hosei_x,zahyou_hosei_y;
      zahyou_hosei_x = zahyou_hosei_y = 0;
      bool flag_fin = false;
      for(int j = 0; j < move_cource[i].size(); j++){
        if(move_cource[i][j].first == 1){
          hosei(zahyou_hosei_x,zahyou_hosei_y,i,j);
        }
        if(mason[n].first+zahyou_hosei_x >= 1 & mason[n].second+zahyou_hosei_y >= 1){
          if(mason[n].second+zahyou_hosei_x < height-1 & mason[n].second+zahyou_hosei_y < width-1){
            if(board[mason[n].second+zahyou_hosei_x][mason[n].second+zahyou_hosei_y] == 1){
              flag_fin = true;
              break;
            }
          }
          else{
            flag_fin = true;
            break;
          }
        }
        else{
          flag_fin = true;
          break;
        }

        if(j == move_cource[i].size()-1){
          score[n][i] = 50;
          continue;
        }
      }
      if(flag_fin){
        score[n][i] = 0;
        continue;
      }
    }
  }
  else if(m == 2){//行動中の障害物判定//tuple(0:行動中のパターン1:現在行動ステップ 2:終了ステップ 3:行動中表示 4:障害物回避行動中表示)
		if(move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].first == 1){
			if(walls[mason[n].first + move_pat[move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second].first][mason[n].second + move_pat[move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second].second] == 0){
				result_move[n] = {move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].first,move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second};
			}
			else if(walls[mason[n].first + move_pat[move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second].first][mason[n].second + move_pat[move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second].second] == 1){

			}
			else if(walls[mason[n].first + move_pat[move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second].first][mason[n].second + move_pat[move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second].second] == 2){
				if(move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second%2){
					int move_em = move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second+1;
					int crash_em = move_cource[get<0>(mason_info[n])][get<1>(mason_info[n])].second-1;
					if(crash_em == 0){
						crash_em = 8;
					}
					
					if(board){//池のとき

					}
					else if(walls[mason[n].first+move_pat[move_em].first][mason[n].second+move_pat[move_em].second] != 2){
						result_move[n] = {1,move_em};
						emagency_move[n].push({1,crash_em});
						emagency_move[n].push({3,crash_em});
						get<4>(mason_info[n]) = true;
					}
					else{
						result_move[n] = {3,move_em};
						emagency_move[n].push({2,move_em});
						emagency_move[n].push({3,crash_em});
						emagency_move[n].push({1,crash_em});
						get<4>(mason_info[n]) = true;
					}
				}
			}
		}
  }
	else if(m == 3){//緊急回避行動解析用

	}

  else if(m == 4){//動けないときの応急処置

  }
}

/*方向
  無方向:0
  左上:1
  上:2
  右上:3
  右:4
  右下:5
  下:6
  左下:7
  左:8*/

void hosei(int &hosei_x, int &hosei_y,int cource, int move_turn){
  if(move_cource[cource][move_turn].second == 1){
    hosei_x -= 1;
    hosei_y -= 1;
  }
  else if(move_cource[cource][move_turn].second == 2){
    hosei_x -= 1;
  }
  else if(move_cource[cource][move_turn].second == 3){
    hosei_x -= 1;
    hosei_y += 1;
  }
  else if(move_cource[cource][move_turn].second == 4){
    hosei_y += 1;
  }
  else if(move_cource[cource][move_turn].second == 5){
    hosei_x += 1;
    hosei_y += 1;
  }
  else if(move_cource[cource][move_turn].second == 6){
    hosei_x += 1;
  }
  else if(move_cource[cource][move_turn].second == 7){
    hosei_x += 1;
    hosei_y -= 1;
  }
  else if(move_cource[cource][move_turn].second == 8){
    hosei_y -= 1;
  }
}



void progress_debug(int turn_l){
  int fin = (((double)turn/(double)turn_max)*100.0);
        cout << '[';
        for(int i = 0; i < 100; i++){
          if(i < fin){
            cout << '*';
          }
          else{
            cout << '-';
          }
        }
        cout << ']';
        cout << ((double)turn/(double)turn_max)*100.0 << '%' << '(' << turn << '/' << turn_max << ')' << endl;
      }

int main(){
  system("python3 get_first.py");
  kaiseki();//盤面情報解析

    /*pybind11::scoped_interpreter interpreter;
    auto m = pybind11::module::import("get_loop");
    auto result = m.attr("f")().cast<int>();*/

    int turn_l = 0;//前回処理時のターン
    if(!first){
      turn_l++;
    }
    while(turn != turn_max){
      system("python3 get_loop.py");
      kaiseki_loop();

      if(turn_l == turn){
        tokutei();

        move();
        
        progress_debug(turn_l);

        turn_l += 2;
      }
    }

  /*(デバッグ用)
  for(int i = 1; i <= people; i++){
    cout << mason[i].first << " " << mason[i].second << endl;
  }

  for(int i = 1; i <= people; i++){
    cout << vsmason[i].first << " " << vsmason[i].second << " " << endl;
  }*/
}