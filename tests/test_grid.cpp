#include "ToastTest.h"
#include "ToastGrid.h"

using namespace Toast;

TOAST_TEST(grid_out_of_range_tiles_are_ignored)
{
	Grid grid(64, 64, 32, 32);
	grid.setTile(1, 1, true);

	grid.setTile(-1, 0, true);
	grid.setTile(0, 2, true);
	grid.setTile(5, 5, true);

	CHECK(grid.getTile(1, 1));
	CHECK(!grid.getTile(0, 0));
	CHECK(!grid.getTile(-1, 0));
	CHECK(!grid.getTile(2, 0));
	CHECK(!grid.getTile(0, -3));
}

TOAST_TEST(grid_bitstring_rows_set_only_their_own_bits)
{
	Grid grid(4 * 32, 2 * 32, 32, 32);
	grid.loadFromString("0011\n0100", "", "\n");

	CHECK(!grid.getTile(0, 0));
	CHECK(!grid.getTile(1, 0));
	CHECK(grid.getTile(2, 0));
	CHECK(grid.getTile(3, 0));
	CHECK(!grid.getTile(0, 1));
	CHECK(grid.getTile(1, 1));
}

TOAST_TEST(grid_csv_rows_keep_their_last_column)
{
	Grid grid(3 * 32, 1 * 32, 32, 32);
	grid.loadFromString("0,0,1", ",", "\n");

	CHECK(!grid.getTile(0, 0));
	CHECK(grid.getTile(2, 0));
}

TOAST_TEST(grid_copy_is_independent_of_the_original)
{
	Grid original(64, 64, 32, 32);
	original.setTile(0, 0, true);

	Grid copy(original);
	copy.setTile(1, 1, true);
	original.setTile(0, 0, false);

	CHECK(copy.getTile(0, 0));
	CHECK(copy.getTile(1, 1));
	CHECK(!original.getTile(1, 1));
}
