#include "ToastTest.h"
#include "TF.h"

using namespace Toast;

TOAST_TEST(tf_resolve_path_is_relative_to_executable)
{
	CHECK(TF::resolvePath("tiles.png") == std::string(SDL_GetBasePath()) + "tiles.png");
	CHECK(TF::resolvePath("/abs/tiles.png") == "/abs/tiles.png");
}

TOAST_TEST(tf_texture_cache_miss_does_not_insert)
{
	CHECK(TF::getTexture("missing.png") == NULL);
	CHECK(TF::textures.empty());
}
