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
#include "Server.hpp"
#include <iostream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include <vector>

/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
//int get_file_length(ifstream *file){
//}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    ifstream file;
    file.open(p1_setup_board); // open the file
    if (!file) // file doesn't exist
    {
        throw ServerException("You're not my mom");
    }
    string line;
    int LineCount = 0;
    while (file >> line) {
        cout << line << endl;
        if (line.length() == board_size) {
            LineCount++;
        } else {
            throw ServerException("You're not me dad either");
        }
    }
    if (LineCount != board_size) {
        throw ServerException("NEVER!");
    }
    file.close();
    // player 2 engage
    file.open(p2_setup_board);
    if (!file) // file doesn't exist
    {
        throw ServerException("You're not my step-mom!");
    }
    string line2;
    int LineCount2 = 0;
    while (file >> line2) {
        if (line2.length() == board_size) {
            LineCount2++;
        } else {
            throw ServerException("You're not me step-dad either");
        }
    }
    if (LineCount2 != board_size) {
        throw ServerException("Sorry but no");
    }
    file.close();
    this->board_size = board_size;
}


Server::~Server() {
}


BitArray2D *Server::scan_setup_board(string setup_board_name){
}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    string number = to_string(player);
    //string number2;

    int number3 = 1;
    if (player == 1){
        number3++;
    }
    //cout << "evalute shot" << endl;

    if (player > 2 || player < 1) {
        throw ServerException("You say you have how many players??");
    }
    else if (x >= board_size || x < 0 || y >= board_size || y < 0) {
        return OUT_OF_BOUNDS;
    }

    else {
        //cout << "else hit eval" << endl;
        //cout << player << "< ---- player number" << endl;
        string number4 = to_string(number3);
        //cout << number4 << "<----- number4" << endl;
        //string number2 = "2";
        //cout << number2 << " <----number2" << endl;
        string scrub = "player_" + number4 + ".setup_board.txt";
        //cout << scrub << "<-----scrub" << endl;
        ifstream shot(scrub);
        if (!shot) cerr << "can't open that file";
        string line;

        vector<vector<char>> test(board_size, vector<char>(board_size));
        //while (!shot.eof()) {
        for(int i = 0; i < test.size(); i++){
            //vector<string> tmpVec;
            //string tmpString;
            for(int j = 0; j < test[i].size(); j++){
                shot >> test[i][j];
                //shot >> tmpString;
                //tmpVec.push_back(tmpString);
                //cout << test[i][j] << endl;
            }
            //test.push_back(tmpVec);
        }
        //}
        shot.close();
        char hit;
        //cout << test[y][x] << "<--------This is the location"<< endl;
        hit = test[y][x];
        if (hit == 'B' || hit == 'C' || hit == 'D' || hit == 'R' || hit == 'S') {
            //cout << "hit" << endl;
            return HIT;
        }
        else {
            return MISS;
            //cout << "miss" << endl;

        }
    }
}


int Server::process_shot(unsigned int player) {
    string number = to_string(player);
    if (player > 2) {
        throw ServerException("Wow really?");
    }
    if (player < 1) {
        throw ServerException("Wow really?");
    }
    string oops2 = "player_" + number + ".shot.json";
    int x = 0, y = 0;
    int result;

    ifstream file;
    file.open(oops2);
    if (!file) {
        return NO_SHOT_FILE;
    }
    //cout << "second" << endl;
    //{
    cereal::JSONInputArchive shoot(file);
    shoot(x, y);
    //cout << "test 3" << endl;
    if ((x < board_size || x > -1) && (y < board_size || y > -1)) {
        //cout << player << "<-----player in proc shot" << endl;
        //cout << "if" << endl;
        if (player == 1) {
            //cout << "player should be 1" << endl;
            //cout << x << endl;
            //cout << y << endl;
            result = evaluate_shot(player, x, y);
        }
        else {
            //cout << "player should be 2" << endl;
            result = evaluate_shot(player + 1, x, y);
        }

    }
    else{
        //cout << "esle" << endl;
        result = OUT_OF_BOUNDS;
    }
    //}
    file.close();
    //cout << "end" << endl;
    string filemetimbers = "player_" + number + ".result.json";
    ofstream fout(filemetimbers);
    {
        cereal::JSONOutputArchive whatever(fout);
        whatever(CEREAL_NVP(result));
    }
    remove(oops2.c_str());
    return SHOT_FILE_PROCESSED;

}