//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include <string>
#include <vector>

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){
    string number = to_string(player);

    if (player > 2 || player < 1) {
        throw ClientWrongPlayerNumberException();
    }
    else{
        this->board_size = board_size;
        this->player = player;

        string oops = "player_" + number + ".action_board.json";

        vector<int>v(board_size, 0);
        vector<vector<int>>board(board_size, v);

        ofstream file(oops);
        //file.open(oops);      //ofstream::out;
        {
            cereal::JSONOutputArchive arc(file);
            arc(CEREAL_NVP(board));
        }
        //file.close();
        initialized = true;

    }
}


void Client::fire(unsigned int x, unsigned int y) {
    string number = to_string(player);
    string fileName = "player_" + number + ".shot.json";

    if(x> board_size || y > board_size || x < 0 || y < 0) {
        throw ClientException("No damnit");
    }
    ofstream file(fileName);
    {
        cereal::JSONOutputArchive arc(file);
        arc(CEREAL_NVP(x),CEREAL_NVP(y));
    }
}


bool Client::result_available() {
    string number = to_string(player);
    string fileName = "player_" + number + ".result.json";

    ifstream file(fileName);
    if (!file) {
        return false;
    }
    else {
        return true;
    }
}


int Client::get_result() {
    int reslut;
    string number = to_string(player);
    string fileName = "player_" + number + ".result.json";
    ifstream file;
    file.open(fileName);
    if(!file){
        throw ClientException("Nope");
    }

    cereal::JSONInputArchive arc(file);
    arc(reslut);
    file.close();

    remove(fileName.c_str());
    if(reslut == -1) {
        return MISS;
    }
    else if(reslut == 0) {
        return OUT_OF_BOUNDS;
    }
    else if (reslut == 1){
        return HIT;
    }
    else {
        throw ClientException("Wrong move doo doo head");
    }
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    vector<vector<int>> board(board_size, vector<int>(board_size));
    string number = to_string(player);
    string fileName = "player_" + number + ".action_board.json";
    ifstream file;
    file.open(fileName);
    if(!file){
        throw ClientException("Nope");
    }

    cereal::JSONInputArchive inarc(file);
    inarc(board);
    //cout << board[y][x] << "<---- this is board" << endl;
    board[y][x] = result;

    file.close();

    ofstream outf(fileName);

    cereal::JSONOutputArchive outArchive(outf);
    outArchive(CEREAL_NVP(board));


}


string Client::render_action_board(){
    string number = to_string(player);
    string another = "player_" + number + ".action_board.json";
    vector<vector<int>> test(board_size, vector<int>(board_size));
    ifstream file(another);
    //{
    cereal::JSONInputArchive arc(file);
    arc(test);
    //}
    file.close();

    for(int i = 0; i < this->board_size; i++){

        for(int j = 0; j < this->board_size; j++){
            cout << test[i][j];

        }
    }
}