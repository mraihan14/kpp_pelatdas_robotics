#include <bits/stdc++.h>
using namespace std;

class Robot{
public :
    Robot(string name){this->name=name;};
    void boot(){cout<<name<<" Ready for complete the mission :)"<<endl;}
    int pos_x;
    int pos_y;
    bool valid_grid=0,valid_mission=0,valid_running=0;
    void init_grid(vector<vector<char>> grid){
        this->grid=grid;
        grid_row=this->grid.size();
        grid_column=this->grid[0].size();
        valid_grid=1;
        valid_mission=0;
        valid_running=0;
    }
    void mission(char s, char o, char g,char p,char b, char w){
        if (valid_grid==0){
            cout<<"SOMETHING WENT WRONG, IM CURIOUS... YOUR GRID NOT READY YET"<<endl;
            return;
        }
        valid_mission=1;
        start=s;objective=o;goal=g;path=p;bom=b;walls=w;
    }
    void run_mission(){
        if (valid_mission==0){
            cout<<"SOMETHING WENT WRONG, IM CURIOUS... YOUR GRID NOT READY YET OR YOU FORGOT TO SET UP MISSION"<<endl;
            return;
        }
        valid_running=1;
        init_start_position();
        pos_x=start_pos.x;
        pos_y=start_pos.y;
        position last_pos;
        
        last_pos=bfs(objective);
        obj_pos.x=last_pos.x;
        obj_pos.y=last_pos.y;
        if (last_pos.x==-1)return; // Cant Reach
        move_set_obj=add_move_set(last_pos);
        
        pos_x=last_pos.x;
        pos_y=last_pos.y;

        last_pos=bfs(goal);
        goal_pos.x=last_pos.x;
        goal_pos.y=last_pos.y;
        if (last_pos.x==-1)return; // Cant Reach
        move_set_goal=add_move_set(last_pos);
        init_moveset(grid_row,grid_column);
        init_log(move_set_obj,start_pos,objective);
        init_log(move_set_goal,obj_pos,goal);
    }

    void status_mission(){
        if (valid_running==0){
            cout<<"SOMETHING WENT WRONG, IM CURIOUS... DID YOU RUN THE MISSION YET?"<<endl;
            return;
        }
        step=0;
        cout<<"Initialize Grid ("<<grid_row<<" X "<<grid_column<<")"<<endl;
        show_grid(-1,-1);
        cout<<"START POSITION    : "<<"("<<start_pos.x<<","<<start_pos.y<<")"<<endl;
        cout<<"Task : Capture the flag and go to base"<<endl;
        cout<<"MISSION START - Capture The Flag"<<endl;
        show(log[objective],start_pos);
        cout<<"MISSION STATUS : FLAG CAPTURED AT POSITION ( "<<obj_pos.x<<" , "<<obj_pos.y<<" )"<<endl;
        grid[obj_pos.x][obj_pos.y]='.';
        cout<<"MOVE TO BASE"<<endl;
        show(log[goal],obj_pos);
        cout<<"MISSION STATUS : REACHED BASE AT POSITION ( "<<goal_pos.x<<" , "<<goal_pos.y<<" )"<<endl;
        cout<<"MISSION COMPLETED"<<endl;
    }
    void log_mission(){
        if (valid_running==0){
            cout<<"SOMETHING WENT WRONG, IM CURIOUS... DID YOU RUN THE MISSION YET?"<<endl;
            return;
        }
        cout<<endl<<endl;
        cout<<"LOG MISSION"<<endl;
        step=0;
        cout<<"START POSITION  : ("<<start_pos.x<<","<<start_pos.y<<")"<<endl;
        cout<<"PATH TO FLAG    : ";
        for (int i=0;i<log[objective].size();i++){
            cout<<log[objective][i]<<" ";
            step+=1;
        }
        cout<<endl;
        cout<<"FLAG CAPTURED   : ("<<obj_pos.x<<","<<obj_pos.y<<")"<<endl;
        cout<<"PATH TO BASE    : ";
        for (int i=0;i<log[goal].size();i++){
            cout<<log[goal][i]<<" ";
            step+=1;
        }
        cout<<endl;
        cout<<"BASE REACHED    : ("<<goal_pos.x<<","<<goal_pos.y<<")"<<endl;
        cout<<"TOTAL MOVES     : "<<step<<endl;
    }
private :
    string name;
    struct position{
        int x;
        int y;
    };
    stack<position> move_set;
    stack<position> move_set_obj;
    stack<position> move_set_goal;
    position start_pos;
    position obj_pos;
    position goal_pos;
    int step;
    vector<vector<position>> pred;
    vector<vector<char>> grid;
    vector<vector<bool>> vis;
    map<char,vector<string>> log;
    int grid_row;
    int grid_column;

    void init_vis(int n,int m){
        vis.assign(n,vector<bool>(m,0));
    }
    void init_pred(int n,int m){
        pred.assign(n,vector<position>(m));
    }
    void init_moveset(int n,int m){
        stack<position> empty;
        move_set=empty;
    }
    void init_start_position(){
        for (int i=0;i<grid_row;i++){
            bool found=0;
            for (int j=0;j<grid_column;j++){
                if (grid[i][j]==start){
                    start_pos.x=i;
                    start_pos.y=j;
                    found=1;
                    break;
                }
            }
            if (found)break;
        }
    }
    char start,objective,goal,path,bom,walls;
    stack<position> add_move_set(position pos_t){
        init_moveset(grid_row,grid_column);
        int back_x=pos_t.x;
        int back_y=pos_t.y;
        move_set.push({pos_t.x,pos_t.y});
        while (pos_x!=back_x||pos_y!=back_y){
            position a=pred[back_x][back_y];
            back_x=a.x;
            back_y=a.y;
            move_set.push({back_x,back_y});
        }
        return move_set;
    }
    void show_grid(int x,int y){
        for (int i=0;i<grid_row;i++){
            for (int j=0;j<grid_column;j++){
                if (x==i&&y==j)cout<<"R ";
                else cout<<grid[i][j]<<' ';
            }cout<<endl;
        }cout<<endl;
    }
    void show(vector<string> log_p,position start_pos){
        show_grid(start_pos.x,start_pos.y);
        for (string s:log_p){
            step+=1;
            if (s=="UP")start_pos.x-=1;
            else if (s=="DOWN")start_pos.x+=1;
            else if (s=="RIGHT")start_pos.y+=1;
            else if (s=="LEFT")start_pos.y-=1;
            auto [x,y]=start_pos;
            cout<<"STEP "<<step<<" - MOVE "<<s<<" - Position ( "<<x<<" , "<<y<<" )"<<endl;
            show_grid(x,y);
        }
    }

    void init_log(stack<position> mv,position start_pos,char c){
        vector<string> log_p;
        auto [px,py]=mv.top();
        mv.pop();
        while (!mv.empty()){
            step+=1;
            auto [x,y]=mv.top();
            mv.pop();
            if (x-px==-1){log_p.push_back("UP");}
            else if (x-px==1){log_p.push_back("DOWN");}
            else if (y-py==-1){log_p.push_back("LEFT");}
            else if (y-py==1){log_p.push_back("RIGHT");}
            px=x;
            py=y;
        }
        log[c]=log_p;
    }

    position bfs(char task){
        init_vis(grid_row,grid_column);
        init_pred(grid_row,grid_column);
        queue<position> q;
        q.push({pos_x,pos_y});
        vis[pos_x][pos_y]=1;
        while (!q.empty()){
            auto [x,y]=q.front();
            q.pop();

            if (grid[x][y]==task)return {x,y};
            int dir_x[4]={1,-1,0,0};
            int dir_y[4]={0,0,1,-1};
            for (int i=0;i<4;i++){
                int adj_x=x+dir_x[i];
                int adj_y=y+dir_y[i];
                if (adj_x<0||adj_y<0||adj_x>=grid_row||adj_y>=grid_column||vis[adj_x][adj_y]||grid[adj_x][adj_y]==bom||grid[adj_x][adj_y]==walls)continue;
                vis[adj_x][adj_y]=1;
                q.push({adj_x,adj_y});
                pred[adj_x][adj_y]={x,y};
            }
        }
        return {-1,-1};
    }
};


int main(){

    vector<vector<char>> soal1 = {
        {'#','#','#','#','#','#','#'},
        {'#','S','#','.','.','.','#'},
        {'#','.','#','.','#','G','#'},
        {'#','.','.','.','#','.','#'},
        {'#','#','#','.','.','.','#'},
        {'#','X','.','.','F','.','#'},
        {'#','#','#','#','#','#','#'}
    };

    vector<vector<char>> soal2 = {
        {'#','#','#','#','#','#','#','#','#'},
        {'#','S','.','.','#','.','.','.','#'},
        {'#','.','#','.','#','.','#','G','#'},
        {'#','.','#','.','.','.','#','.','#'},
        {'#','.','.','.','#','.','.','.','#'},
        {'#','#','#','.','#','.','.','#','#'},
        {'#','X','.','.','#','.','.','F','#'},
        {'#','.','#','#','#','X','.','.','#'},
        {'#','#','#','#','#','#','#','#','#'}
    };

    vector<vector<char>> soal3 = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', 'S', '.', '.', '#', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '#', '.', '#', '.', '#', '#', '#', '.', '#'},
        {'#', '.', '#', '.', '.', '.', '#', 'G', '#', '.', '#'},
        {'#', '.', '.', '.', '#', '.', '#', '.', '#', '.', '#'},
        {'#', '#', '#', '.', '#', '.', '.', '.', '#', '.', '#'},
        {'#', '.', '.', '.', '#', '#', '#', '.', '#', '.', '#'},
        {'#', '.', '#', '.', '.', 'X', '.', '.', '#', '.', '#'},
        {'#', '.', '#', '.', '#', '#', '#', '.', '#', 'F', '#'},
        {'#', 'X', '.', '.', '.', '.', 'X', '.', '.', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
    };

    Robot duck_bot("Kwek");

    cout<<"============================SOAL 1================================"<<endl;
    duck_bot.init_grid(soal1);
    duck_bot.mission('S','F','G','.','X','#');
    duck_bot.run_mission();
    duck_bot.status_mission();
    duck_bot.log_mission();
    cout<<"============================  END  ==============================="<<endl;

    cout<<"============================SOAL 2================================"<<endl;
    duck_bot.init_grid(soal2);
    duck_bot.mission('S','F','G','.','X','#');
    duck_bot.run_mission();
    duck_bot.status_mission();
    duck_bot.log_mission();
    cout<<"============================  END  ==============================="<<endl;

    cout<<"============================SOAL 3================================"<<endl;
    duck_bot.init_grid(soal3);
    duck_bot.mission('S','F','G','.','X','#');
    duck_bot.run_mission();
    duck_bot.status_mission();
    duck_bot.log_mission();
    cout<<"============================  END  ==============================="<<endl;
}