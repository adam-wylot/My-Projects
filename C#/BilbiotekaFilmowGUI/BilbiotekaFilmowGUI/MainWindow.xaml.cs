using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Json;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace MoviesListGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ObservableCollection<Movie> movies;

        private async void GetPopularList()
        {
            HttpClient http = new HttpClient();
            var response = await http.GetAsync(@"https://api.themoviedb.org/3/movie/popular?api_key=75e5725b6e3ff72be7570c72055b3837&language=en-US&page=1");
            var responeMovies = await response.Content.ReadFromJsonAsync<PopularMovies>();
            movies = new ObservableCollection<Movie>(responeMovies.results);
            moviesList.ItemsSource = movies;
        }

        public MainWindow()
        {
            InitializeComponent();
            GetPopularList();
        }

        private void moviesList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if(moviesList.SelectedItem is Movie movie)
            {
                new AdditionalWindow(movie).Show();
            }
        }
    }
}
