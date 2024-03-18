using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace SystemMagazynowyMVC.Migrations
{
    public partial class zamowienietowar : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Towary_Zamowienia_ZamowienieId",
                table: "Towary");

            migrationBuilder.DropIndex(
                name: "IX_Towary_ZamowienieId",
                table: "Towary");

            migrationBuilder.DropColumn(
                name: "ZamowienieId",
                table: "Towary");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<int>(
                name: "ZamowienieId",
                table: "Towary",
                type: "int",
                nullable: true);

            migrationBuilder.CreateIndex(
                name: "IX_Towary_ZamowienieId",
                table: "Towary",
                column: "ZamowienieId");

            migrationBuilder.AddForeignKey(
                name: "FK_Towary_Zamowienia_ZamowienieId",
                table: "Towary",
                column: "ZamowienieId",
                principalTable: "Zamowienia",
                principalColumn: "Id");
        }
    }
}
