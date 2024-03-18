using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*{
    "adult": false,
"backdrop_path": "/70nxSw3mFBsGmtkvcs91PbjerwD.jpg",
"genre_ids": [
878,
28,
12
],
"id": 580489,
"original_language": "en",
"original_title": "Venom: Let There Be Carnage",
"overview": "After finding a host body in investigative reporter Eddie Brock, the alien symbiote must face a new enemy, Carnage, the alter ego of serial killer Cletus Kasady.",
"popularity": 8633.976,
"poster_path": "/rjkmN1dniUHVYAtwuV3Tji7FsDO.jpg",
"release_date": "2021-09-30",
"title": "Venom: Let There Be Carnage",
"video": false,
"vote_average": 7.2,
"vote_count": 4398
}*/

namespace MoviesListGUI
{
    public class Movie
    {
        public const string imageUrl = "https://image.tmdb.org/t/p/w500";
        public string title { get; set; }
        public string overview { get; set; }
        public double popularity { get; set; }
        public string poster_path { get; set; }
        public DateTime release_date { get; set; }

        public override string? ToString()
        {
            return title; 

        }
    }
}
