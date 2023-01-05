#include<iostream>
#include<SFML/Graphics.hpp>
#include<vector>

#define MATRIX_SIZE 200
#define WIDTH 300
#define HEIGHT 600
#define LENGTH 30


//colors
std::vector<std::vector<int>> colors;
std::vector<int> curr_color;
std::vector<int> yellow = {255,255,0,255};
std::vector<int> red = {255,0,0,255};
std::vector<int> blue = {0,0,255,255};
std::vector<int> green = {0,255,0,255};
std::vector<int> black = {0,0,0,255};

bool paused = false;

sf::RectangleShape matrix[20][10];

std::vector<std::pair<int,int>> curr_tetris;

std::vector<std::vector<std::vector<int>>> tetris;



int curr_tetris_index = 0;

void create_tetris()
{
    tetris.push_back({{0,1},
                      {1,1},
                      {1,0}});
    
    tetris.push_back({{1},
                      {1},
                      {1}});
        
    tetris.push_back({{1},
                      {1}});

    tetris.push_back({{1,1},
                      {1,1}});

}

void create_colors()
{
    colors.push_back(yellow);
    colors.push_back(blue);
    colors.push_back(green);
    colors.push_back(red);
}

void set_matrix()
{
    int k = 0;
    sf::Vector2f pos(0,0);
    for(int i=0; i<20; i++)
    {
        for(int j=0; j<10; j++)
        {
            pos.x = j*LENGTH;
            pos.y = i*LENGTH;
            matrix[i][j].setPosition(pos);
            matrix[i][j].setSize(sf::Vector2f(LENGTH,LENGTH));
            matrix[i][j].setFillColor(sf::Color::Black);
            matrix[i][j].setOutlineColor(sf::Color::Black);
            matrix[i][j].setOutlineThickness(3);
            k++;
        }
    }
}


void set_tetris(std::vector<std::vector<int>> tetris, int xpos, int ypos)
{
    curr_tetris.clear();
    int k = 0;
    for(int i=0; i<tetris.size(); i++)
    {
        for(int j=0; j<tetris[0].size(); j++)
        {
            if (tetris[i][j]==1)
            {
                std::pair<int,int> p;
                p.first = xpos + j;
                p.second = ypos + i; 
                curr_tetris.push_back(p);
                k++;
            }
        }
    }
}

void set_tetris_color(std::vector<int> color)
{
    int x,y;
    for(int i=0; i<curr_tetris.size(); i++)
    {
        x = curr_tetris[i].first;
        y = curr_tetris[i].second;

        matrix[y][x].setFillColor(sf::Color(color[0],color[1],color[2],color[3]));
    }
}


// if the yoffset is 1 it checks the blocks to the bottom of the current one, and if the xoffset is 1 it checks the blocks to the right of the current
bool check_tetris(int xoffset, int yoffset)
{
    for(int i=0; i<curr_tetris.size(); i++)
    {
        int x = curr_tetris[i].first;
        int y = curr_tetris[i].second;

        if(x+xoffset == -1 || x+xoffset == 10 || y+yoffset == 20)
        {
            return false;
        }

        if(matrix[y+yoffset][x+xoffset].getFillColor() != sf::Color::Black)
        {
            bool is_not_from_tetris = true;
            for(int j=0; j<curr_tetris.size(); j++)
            {
                if(curr_tetris[j].first == x+xoffset && curr_tetris[j].second == y+yoffset)
                {
                    is_not_from_tetris = false;
                }
            }
            if(is_not_from_tetris)
            {
                return false;
    
            }
        }
    }
    return true;
}



void tetris_fall()
{
    if(!check_tetris(0,1))
    {
        curr_color = colors[rand()%4];
        curr_tetris_index = rand() % 4;
        set_tetris(tetris[curr_tetris_index], 4, 0); 
        return;
    }
    set_tetris_color(black);
    for(int i=0; i<curr_tetris.size(); i++)
    {
        curr_tetris[i].second++;
    }

    set_tetris_color(curr_color);
}

void invert_tetris()
{
    set_tetris_color(black);
    std::vector<std::vector<int>> t;
    for(int i=0; i<tetris[curr_tetris_index][0].size(); i++)
    {
        std::vector<int> curr;
        for(int j=tetris[curr_tetris_index].size()-1; j>=0; j--)
        {
            curr.push_back(tetris[curr_tetris_index][j][i]);
        }
        t.push_back(curr);
    }        
    tetris[curr_tetris_index] = t;
    set_tetris(tetris[curr_tetris_index], curr_tetris[0].first, curr_tetris[0].second);
    set_tetris_color(curr_color);
}





void move_right()
{
    if(check_tetris(1,0))
    {
        set_tetris_color(black);
        for(int i=0; i<curr_tetris.size(); i++)
        {
            curr_tetris[i].first++;
        }

        set_tetris_color(curr_color);
    }
}

void move_left()
{
    if(check_tetris(-1,0))
    {
        set_tetris_color(black);
        for(int i=0; i<curr_tetris.size(); i++)
        {
            curr_tetris[i].first--;
        }

        set_tetris_color(curr_color);
    }
}




int main()
{

    sf::RenderWindow window(sf::VideoMode(300,600), "Tetris");
    window.setFramerateLimit(5);

    create_tetris();

    create_colors();

    set_matrix();
    
    set_tetris(tetris[curr_tetris_index], 4, 0);

    curr_color = yellow;
        
    set_tetris_color(curr_color);



    while(window.isOpen())
    {
        window.setKeyRepeatEnabled(false);
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) window.close();

            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) window.close();
                if(event.key.code == sf::Keyboard::Space) invert_tetris();
                if(event.key.code == sf::Keyboard::D) move_right(); 
                if(event.key.code == sf::Keyboard::A) move_left();
            } 
        }

        window.clear();



        tetris_fall();


        for(int i=0; i<20; i++)
        {
            for(int j=0; j<10; j++)
            {
                window.draw(matrix[i][j]);
            }
        }

       

        window.display();

    }
    return 0;
}