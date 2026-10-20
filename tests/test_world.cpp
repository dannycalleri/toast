#include "ToastTest.h"
#include "ToastEntity.h"
#include "ToastWorld.h"

using namespace Toast;

namespace
{
	int deleted = 0;

	class Counter : public Entity
	{
	public:
		Counter(World* world, bool removeSelf) : world(world), removeSelf(removeSelf), updates(0) {}
		virtual ~Counter() { deleted++; }

		virtual void update()
		{
			updates++;
			if(removeSelf)
				world->removeLater(this);
		}

		World* world;
		bool removeSelf;
		int updates;
	};
}

TOAST_TEST(world_entity_can_remove_itself_during_update)
{
	World world;
	Counter* first = new Counter(&world, true);
	Counter* second = new Counter(&world, false);
	Counter* third = new Counter(&world, false);
	world.add(first);
	world.add(second);
	world.add(third);

	deleted = 0;
	world.update();
	CHECK(deleted == 1);
	CHECK(world.entities.size() == 2);
	CHECK(world.entities[0] == second && world.entities[1] == third);
	CHECK(second->updates == 1 && third->updates == 1);
}

TOAST_TEST(world_queued_entity_is_deleted_once)
{
	World world;
	Counter* twice = new Counter(&world, false);
	Counter* removedNow = new Counter(&world, false);
	world.add(twice);
	world.add(removedNow);

	deleted = 0;
	world.removeLater(twice);
	world.removeLater(twice);
	world.removeLater(removedNow);
	world.remove(removedNow);
	CHECK(deleted == 1);

	world.update();
	CHECK(deleted == 2);
	CHECK(world.entities.empty());

	world.update();
	CHECK(deleted == 2);
}
