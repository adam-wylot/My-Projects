using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace SystemMagazynowyMVC.Migrations
{
    public partial class zamowienietowar2 : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "ZamomwioneTowary",
                columns: table => new
                {
                    Id = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    TowarId = table.Column<int>(type: "int", nullable: false),
                    ZamowienieId = table.Column<int>(type: "int", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_ZamomwioneTowary", x => x.Id);
                    table.ForeignKey(
                        name: "FK_ZamomwioneTowary_Towary_TowarId",
                        column: x => x.TowarId,
                        principalTable: "Towary",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_ZamomwioneTowary_Zamowienia_ZamowienieId",
                        column: x => x.ZamowienieId,
                        principalTable: "Zamowienia",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_ZamomwioneTowary_TowarId",
                table: "ZamomwioneTowary",
                column: "TowarId");

            migrationBuilder.CreateIndex(
                name: "IX_ZamomwioneTowary_ZamowienieId",
                table: "ZamomwioneTowary",
                column: "ZamowienieId");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "ZamomwioneTowary");
        }
    }
}
