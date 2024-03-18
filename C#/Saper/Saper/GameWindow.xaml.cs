using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Saper
{
    /// <summary>
    /// Logika interakcji dla klasy gameWindow.xaml
    /// </summary>
    public partial class GameWindow : Window
    {
        //Zmienne
        Random rand;
        private Button[,] buttons;
        private Label[,] labels;
        private Timer aTimer;

        private int[,] mapInfo;
        private int mapSize;
        private int startBombsAmount;
        private int bombsAmount;
        private bool[,] wasHere;
        private bool[,] flagged;
        private bool firstClick;
        private bool gameIsOver;
        private int time;

        //zmienne potrzebne do tworzenia mapy (globalne)
        private int leftBombs;
        private int leftPools;

        //Konstruktor
        public GameWindow(int size, int bombs)
        {
            InitializeComponent();

            rand = new Random();
            buttons = new Button[size, size];
            labels = new Label[size, size];

            mapSize = size;
            startBombsAmount = bombs;

            time = 0;
            aTimer = new Timer(1000);
            aTimer.Elapsed += OnTimedEvent;
            aTimer.AutoReset = true;

            //Rozdzielanie Grid'u
            for (int i = 0; i < mapSize; i++)
            {
                gameGrid.RowDefinitions.Add(new RowDefinition());
                gameGrid.ColumnDefinitions.Add(new ColumnDefinition());
            }
            gameWindow.Width = mapSize * 25;
            gameWindow.Height = mapSize * 25 + 50;

            MapPreparation();
        }

        //Metody
        private void MapPreparation()
        {
            firstClick = true;
            gameIsOver = false;
            mapInfo = new int[mapSize, mapSize];
            wasHere = new bool[mapSize, mapSize];
            flagged = new bool[mapSize, mapSize];
            resetButton.Content = ":)";
            bombsAmount = startBombsAmount;
            leftBombsLabel.Content = bombsAmount.ToString();
            gameGrid.Children.Clear();

            leftBombs = bombsAmount;
            leftPools = mapSize * mapSize;

            //Tworzenie przycisków
            for (int i = 0; i < mapSize; i++)
            {
                for (int j = 0; j < mapSize; j++)
                {
                    buttons[j, i] = new Button();
                    buttons[j, i].Tag = $"{j}-{i}";
                    //funkcje
                    buttons[j, i].Click += Game_Button_Click;
                    buttons[j, i].MouseRightButtonDown += Flag_Button_Click;
                    //grid
                    gameGrid.Children.Add(buttons[j, i]);
                    Grid.SetColumn(buttons[j, i], j);
                    Grid.SetRow(buttons[j, i], i);
                }
            }
        }

        private void AreaSecuring(int x, int y)
        {
            int xLimit = x + 3;
            int yLimit = y + 3;

            if (x < 0)              x = 0;
            if (y < 0)              y = 0;
            if (xLimit > mapSize)   xLimit = mapSize;
            if (yLimit > mapSize)   yLimit = mapSize;

            for (int i = y;  i < yLimit; i++)
            {
                for (int j = x;  j < xLimit; j++)
                {
                    if(mapInfo[j, i] > -1)
                    {
                        mapInfo[j, i]++;
                    }
                }
            }
        }

        private void GameOver()
        {
            gameIsOver = true;
            resetButton.Content = ":(";

            //Pokazywanie bomb
            for (int i = 0; i < mapSize; i++)
            {
                for (int j = 0; j < mapSize; j++)
                {
                    if(mapInfo[j, i] == -1)
                    {
                        buttons[j, i].Content = "*";
                    }
                }
            }
        }
        private void ShowTerrain(int x, int y)
        {
            if (mapInfo[x, y] > -1)
            {
                bool eqZero = true;
                gameGrid.Children.Remove(buttons[x, y]);
                
                if (mapInfo[x, y] > 0)
                {
                    eqZero = false;
                    labels[x, y] = new Label();
                    labels[x, y].Content = mapInfo[x, y].ToString();
                    labels[x, y].FontSize = 12;
                    gameGrid.Children.Add(labels[x, y]);
                    Grid.SetRow(labels[x, y], y);
                    Grid.SetColumn(labels[x, y], x);
                }
                mapInfo[x, y] = -10;

                if (eqZero)
                {
                    if (y > 0) ShowTerrain(x, y - 1);
                    if (x < mapSize - 1 && y > 0) ShowTerrain(x + 1, y - 1);
                    if (x < mapSize - 1) ShowTerrain(x + 1, y);
                    if (x < mapSize - 1 && y < mapSize - 1) ShowTerrain(x + 1, y + 1);
                    if (y < mapSize - 1) ShowTerrain(x, y + 1);
                    if (x > 0 && y < mapSize - 1) ShowTerrain(x - 1, y + 1);
                    if (x > 0) ShowTerrain(x - 1, y);
                    if (x > 0 && y > 0) ShowTerrain(x - 1, y - 1);
                }
            }
        }

        private void FirstClick(int x, int y)
        {
            leftPools--;
            wasHere[x, y] = true;

            if (y > 0) StartSpace(x, y - 1, perc: 100);
            if (x < mapSize - 1 && y > 0) StartSpace(x + 1, y - 1, perc: 100);
            if (x < mapSize - 1) StartSpace(x + 1, y, perc: 100);
            if (x < mapSize - 1 && y < mapSize - 1) StartSpace(x + 1, y + 1, perc: 100);
            if (y < mapSize - 1) StartSpace(x, y + 1, perc: 100);
            if (x > 0 && y < mapSize - 1) StartSpace(x - 1, y + 1, perc: 100);
            if (x > 0) StartSpace(x - 1, y, perc: 100);
            if (x > 0 && y > 0) StartSpace(x - 1, y - 1, perc: 100);
        }

        private void StartSpace(int x, int y, int perc)
        {
            if (perc >= rand.Next(1, 100) && !wasHere[x, y])
            {
                leftPools--;
                wasHere[x, y] = true;

                if (y > 0) StartSpace(x, y - 1, perc / 3 * (leftPools - leftBombs) / leftPools);
                if (x < mapSize - 1) StartSpace(x + 1, y, perc / 3 * (leftPools - leftBombs) / leftPools);
                if (y < mapSize - 1) StartSpace(x, y + 1, perc / 3 * (leftPools - leftBombs) / leftPools);
                if (x > 0) StartSpace(x - 1, y, perc / 3 * (leftPools - leftBombs) / leftPools);
            }
        }

        private void MapMaker()
        {
            for (int i = 0; i < mapSize; i++)
            {
                for (int j = 0; j < mapSize; j++)
                {
                    if (!wasHere[j, i])
                    {
                        //Stawianie bomb
                        if (10000 * leftBombs / leftPools >= rand.Next(1, 10000))
                        {
                            mapInfo[j, i] = -1; //bomba
                            AreaSecuring(j - 1, i - 1);
                            leftBombs--;
                        }
                        leftPools--;
                        wasHere[j, i] = true;
                    }
                }
            }
        }
        private void OnTimedEvent(Object source, ElapsedEventArgs e)
        {
            time++;
            ChangeTime();
        }

        private void ChangeTime()
        {
            l_time.Content = time.ToString();
        }

        //Przyciski
        private void Game_Button_Click(object sender, RoutedEventArgs e)
        {
            Button? b = sender as Button;
            if (b != null && !gameIsOver)
            {
                string[] arr = b.Tag.ToString().Split("-");
                int x = int.Parse(arr[0]);
                int y = int.Parse(arr[1]);

                if (!flagged[x, y])
                {
                    if (firstClick)
                    {
                        FirstClick(x, y);
                        MapMaker();
                        firstClick = false;
                        aTimer.Enabled = true;
                    }

                    if (mapInfo[x, y] != -1)
                    {
                        ShowTerrain(x, y);
                    }
                    else
                    {
                        GameOver();
                    }
                }
            }
        }
        private void Flag_Button_Click(object sender, MouseButtonEventArgs e)
        {
            Button? b = sender as Button;
            if (b != null && !gameIsOver && !firstClick)
            {
                string[] arr = b.Tag.ToString().Split("-");
                int x = int.Parse(arr[0]);
                int y = int.Parse(arr[1]);

                if(flagged[x, y])
                {
                    buttons[x, y].Content = "";
                    flagged[x, y] = false;
                }
                else
                {
                    buttons[x, y].Content = "?";
                    flagged[x, y] = true;
                }
            }
        }
        private void Reset_Button_Click(object sender, RoutedEventArgs e)
        {
            MapPreparation();
        }
    }
}
