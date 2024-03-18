using System;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace SystemMagazynowyMVC.Migrations
{
    public partial class init : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Zamowienia",
                columns: table => new
                {
                    Id = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Imie = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Nazwisko = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Adres = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Dostarczenie = table.Column<int>(type: "int", nullable: false),
                    DataRealizacji = table.Column<DateTime>(type: "datetime2", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Zamowienia", x => x.Id);
                });

            migrationBuilder.CreateTable(
                name: "Towary",
                columns: table => new
                {
                    Id = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Kod = table.Column<int>(type: "int", nullable: false),
                    Nazwa = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Gwarancja = table.Column<DateTime>(type: "datetime2", nullable: false),
                    Typ = table.Column<int>(type: "int", nullable: false),
                    Ilosc = table.Column<int>(type: "int", nullable: false),
                    ZamowienieId = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Towary", x => x.Id);
                    table.ForeignKey(
                        name: "FK_Towary_Zamowienia_ZamowienieId",
                        column: x => x.ZamowienieId,
                        principalTable: "Zamowienia",
                        principalColumn: "Id");
                });

            migrationBuilder.CreateIndex(
                name: "IX_Towary_ZamowienieId",
                table: "Towary",
                column: "ZamowienieId");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Towary");

            migrationBuilder.DropTable(
                name: "Zamowienia");
        }
    }
}
