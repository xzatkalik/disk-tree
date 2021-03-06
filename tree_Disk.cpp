#include "headers.h"
#include "tree_Disk.h"
#include "tree_Folder.h"
#include "tree_Link.h"

#include <stack>

#ifdef _DEBUG
#define new DBG_NEW
#endif

tree::Folder * tree::ParseDisk(rapidjson::Value & json)
{
	// parse disk hierarchy
	Folder * root = dynamic_cast<Folder*>(Folder::Parse(json));
	if (!root)
		return nullptr;

	// resolve links
	std::stack<Folder *> folders;

	folders.push(root);

	while (!folders.empty())
	{
		Folder * folder = folders.top();
		folders.pop();

		for (auto * node : folder->Content())
		{
			if (auto * subfolder = dynamic_cast<Folder*>(node))
			{
				folders.push(subfolder);
			}
			else if (auto * link = dynamic_cast<Link *>(node))
			{
				Node * node = root->Find(link->Path());
				if (node)
					link->Set(node);
			}
		}
	}

	return root;
}
