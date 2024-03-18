using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace MoviesListGUI
{
    /// <summary>
    /// Logika interakcji dla klasy AdditionalWindow.xaml
    /// </summary>
    public partial class AdditionalWindow : Window
    {
        public AdditionalWindow(Movie movie)
        {
            InitializeComponent();
            LoadDetails(movie);
        }

        private void LoadPoster(string url)
        {
            try
            {
                WebClient webClient = new WebClient();
                BitmapImage bitmapImage = new BitmapImage();
                bitmapImage.BeginInit();
                bitmapImage.UriSource = new Uri(Movie.imageUrl+url, UriKind.Absolute);
                bitmapImage.EndInit();
                posterImage.Source = bitmapImage;
            }
            catch (Exception) //WebException
            {
                posterImage.Source = new BitmapImage(new Uri(AppDomain.CurrentDomain.BaseDirectory + "/noImage.jpg"));
            }
        }

        private void LoadDetails(Movie movie)
        {
            LoadPoster(movie.poster_path);

            titleLabel.Content = movie.title;
            window.Title = movie.title;
            descriptionText.Text = movie.overview;
            popularityLabel.Content = "Popularity: " + movie.popularity;
            releaseDateLabel.Content = "Release Date: " + movie.release_date.ToShortDateString();
        }
    }
}
