namespace ProgramKalkulacyjnyGUI
{
    public class RecentFileProperties
    {
        private string name;
        private string path;

        public string Name { get => name; set => name = value; }
        public string Path { get => path; set => path = value; }

        //Konstruktor
        public RecentFileProperties()
        {

        }
        public RecentFileProperties(string name, string path)
        {
            this.name = name;
            this.path = path;
        }

        public override string? ToString()
        {
            return name;
        }
    }
}
