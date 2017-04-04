/*
 *(c) Zachary Job
 *
 *defs_internal.h
 *
 *Generic algorithms and structures reside here
 *For example tree structures


NOTE - FORGOT TO IMPLEMENT FILE UPDATES...
NOTE - FORGOT TO IMPLEMENT FILE UPDATES...
NOTE - FORGOT TO IMPLEMENT FILE UPDATES...
NOTE - FORGOT TO IMPLEMENT FILE UPDATES...

 
 *Some of these structures will not submit to
 *debugging standards to lower unneeded overhead
 */

#ifndef DEFS_INTERNAL
#define DEFS_INTERNAL

#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>
#include <stack> 

#include "defs_file.h"
#include "defs_filesystem.h"
#include "defs_encryption.h"

#define INTERNAL_UNBALANCED_TREE_FILESYSTEM_BROADCAST_IDENTIFIERp "-1"

using namespace std;

/*
 *A struct containing the required members for use
 *in any node data to be passed to ineternal 
 *unbalanced tree
 */
struct internal_unbalanced_tree_node_parent
{
	bool
		is_directory;

	string
		name;
};

/*
 *A structure intended for use in representing
 *the host filesystem and other uses
 *
 * NOTE: the node_data_T must provide "name"
 *
 *The node data type is dedicated, ;this heap is
 *intended for uniform types
 */
template <typename node_data_T>
class internal_unbalanced_tree
{
	private:

		struct internal_node_container_struct
		{
			struct internal_node_container_struct *parent;
			//The boolean represents if the child is a directory
			vector<struct internal_node_container_struct> children;

			node_data_T element;

			internal_node_container_struct();
		};
		typedef struct internal_node_container_struct internal_node_container;

		struct internal_node_container_struct
			internal_head,
			*internal_heap_pointer;

		stack<struct internal_node_container_struct*>
			internal_bfs_stack;

		friend class internal_unbalanced_tree_filesystem;
			
		inline auto bfsStep();
		inline void bfsClear();

	public:	
	
		internal_unbalanced_tree();
		~internal_unbalanced_tree();

		bool addNode(struct internal_node_container_struct child);
		bool removeNode(string name = "");
		shared_ptr<node_data_T> getNode(string name = "");
		bool moveUp();
		bool moveDown(string name);
		void clean();
};

/*
 *A structure intended for use in representing
 *the host filesystem and other uses
 *
 * WRAPPING MUST BE DONE ABOVE to cure 
 * calls to the .. operator
 *
 *The redun. function similarities will be
 *compacted post optimization
 */
class internal_unbalanced_tree_filesystem
{
	private:

		bool
			internal_offset_armed;

		string
			internal_filesystem_offset,
			internal_host_identifier;

		struct internal_node_data_struct : internal_unbalanced_tree_node_parent
		{
			bool
				is_foreign;
			
			string
				host,
				lookup;

			shared_ptr<char>
				file_content;

			internal_node_data_struct();
			internal_node_data_struct(bool is_foreign, bool is_directory, string lookup, string host = "", shared_ptr<char> file_content = nullptr);
			void set(bool is_foreign, bool is_directory, string lookup, string host = "", shared_ptr<char> file_content = nullptr);
		};
		typedef struct internal_node_data_struct internal_node_data;

		//FOR optimization later
		//list<internal_node_data>
		//	internal_current_path;
		//
		map<string, string>
			internal_foreign_nodes;

		filesystem_head
			internal_filesystem_reader;

		internal_unbalanced_tree<internal_node_data>
			internal_filesystem_tree;
	
		string pathChangeInline(string path, bool keep_move);	

	public:
	
		internal_unbalanced_tree_filesystem();
		~internal_unbalanced_tree_filesystem();

		bool setFilesystemOffset(string offset);
		bool updateRepresentation();
		
		string serializeRequest(string lookup);
		void deserializeResponse(string serialized_response);
		string serializeResponse(string serialized_request);

		string getForeignLookup(string lookup = "");
		void addForeignLookup(string path = "", string host = INTERNAL_UNBALANCED_TREE_FILESYSTEM_BROADCAST_IDENTIFIERp);
		bool removeForeignLookup(string lookup = "");

		bool killParent();
		bool killChild(string name);

		bool pathExists(string path);
		bool stepBackwardsOnce();
		bool stepForwardsOnce(string name);
		bool pathChange(string path);

		void setHostIdentifier(string identifier);

		void clean();
};

//INTERNAL UNBALANCED TREE///////////////////
/////////////////////////////////////////////

/*
 *An initializer for the node data
 *element
 */
template <typename node_data_T>
internal_unbalanced_tree<node_data_T>::internal_node_container_struct::internal_node_container_struct():
	parent(nullptr)
{
}

/*
 *Utilized for moving the head according to a 
 *breadth first search algorithm
 *
 * Note, I am abusing auto in to develop some
 * new habits
 */
template <typename node_data_T>
auto internal_unbalanced_tree<node_data_T>::bfsStep()
{
	internal_node_container
		*result;

	result = nullptr;

	//An iteration of the traversal has started
	if(!internal_bfs_stack.empty())
	{
		auto get_top = internal_bfs_stack.top();
		internal_bfs_stack.pop();

		//The current iterator contains children
		if(!get_top->children.empty())
		{
			for(auto get_iterator : get_top->children)
				internal_bfs_stack.emplace(&get_iterator);
		}

		result = get_top;
	}
	else
	{
		for(auto get_iterator : internal_heap_pointer->children)
			internal_bfs_stack.emplace(&get_iterator);
		result = internal_heap_pointer;
	}

	return result;
}

/*
 *Clear the stack
 */
template <typename node_data_T>
void internal_unbalanced_tree<node_data_T>::bfsClear()
{
	while(!internal_bfs_stack.empty())
		internal_bfs_stack.pop();
}

/*
 *Generic initializer
 */
template <typename node_data_T>
internal_unbalanced_tree<node_data_T>::internal_unbalanced_tree():
	internal_heap_pointer(&internal_head)
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
template <typename node_data_T>
internal_unbalanced_tree<node_data_T>::~internal_unbalanced_tree()
{
}

/*
 *Add a node to the current parent
 *
 *@PARAM: The child node to add
 *@RETURN: If the node was added
 */
template <typename node_data_T>
bool internal_unbalanced_tree<node_data_T>::addNode(struct internal_node_container_struct child)
{
	bool
		result;

	result = true;

	internal_heap_pointer->children.emplace(internal_heap_pointer->children.end(), child);
	
	return result;
}

/*
 *Remove a specific node, no input specifies the parent
 *
 * ONLY WORKS FOR THE CURRENT LEVEL
 *
 *@PARAM: The name of the node to remove
 *@RETURN: If the node was removed
 */
template <typename node_data_T>
bool internal_unbalanced_tree<node_data_T>::removeNode(string name)
{
	bool
		result;
	
	result = true;

	if(name != "")
	{
		auto get_iterator = find_if(	internal_heap_pointer->children.begin(),
												internal_heap_pointer->children.end(), 
												[name](auto child){return child.element.name == name;});
		result = get_iterator != internal_heap_pointer->children.end();

		if(result)
			(internal_heap_pointer->children).erase(get_iterator);
	}
	else
	{
		if(!internal_heap_pointer->children.empty())
			internal_heap_pointer->children.clear();
		if(internal_heap_pointer != &internal_head)
		{
			auto get_pointer = internal_heap_pointer;
			internal_heap_pointer = internal_heap_pointer->parent;

			auto get_iterator = find_if(	internal_heap_pointer->children.begin(),
													internal_heap_pointer->children.end(), 
													[get_pointer](auto child){return child.element.name == get_pointer->element.name;});

			(internal_heap_pointer->children).erase(get_iterator);
		}
		//else
		//setelem?

		result = true;
	}

	return result;
}

/*
 *Retreive the node currenlty rested on
 *
 *@PARAM: The name of the node to retreive
 *@RETURN: The data retreived
 */
template <typename node_data_T>
shared_ptr<node_data_T> internal_unbalanced_tree<node_data_T>::getNode(string name)
{
	shared_ptr<node_data_T>
		result;
	
	result = nullptr;

	if(name == "")
		result = make_shared<node_data_T>(internal_heap_pointer->element);
	else
	{
		auto get_iterator = find_if(	internal_heap_pointer->children.begin(),
												internal_heap_pointer->children.end(), 
												[name](auto child){return child.element.name == name;});

		if(get_iterator != internal_heap_pointer->children.end())
			result = make_shared<node_data_T>(get_iterator->element);
	}

	return result;
}

/*
 *Traverse up the tree, or backwards, or slantwise
 *where the vertical vector is upwards biased...
 *
 *@RETURN: If the upwards traversal succeded
 */
template <typename node_data_T>
bool internal_unbalanced_tree<node_data_T>::moveUp()
{
	bool
		result;

	result = false;

	if(internal_heap_pointer->parent != nullptr)
	{
		internal_heap_pointer = internal_heap_pointer->parent;
		result = true;
	}

	return result;
}

/*
 *Traverse downwards on the tree by specifying the
 *desired child. Perhaps this one isn't slantwise
 *at times... I'll stop
 *
 *@PARAM: The node to travel down to
 *@RETURN: If the node was traveled to
 */
template <typename node_data_T>
bool internal_unbalanced_tree<node_data_T>::moveDown(string name)
{
	bool
		result;

	result = false;

	auto get_iterator = find_if(	internal_heap_pointer->children.begin(),
											internal_heap_pointer->children.end(), 
											[name](auto child){return (bool)(child.element.name == name);});
	result = get_iterator != internal_heap_pointer->children.end();

	if(result)
		internal_heap_pointer = &*get_iterator;

	return result;
}

/*
 *Purge the structure
 */
template <typename node_data_T>
void internal_unbalanced_tree<node_data_T>::clean()
{
	auto get_current = bfsStep();
	get_current->children.clear();
	//get_current->element set clean?

	while(!internal_bfs_stack.empty())
	{
		get_current = bfsStep();
		get_current->children.clear();
		//get_current->element set clean?
	}
}

//INTERNAL UNBALANCED TREE FILESYSTEM////////
/////////////////////////////////////////////

/*
 *For use in creating the node data structure
 *This is for expansion if needed
 */
internal_unbalanced_tree_filesystem::internal_node_data_struct::internal_node_data_struct():
	host(""),
	file_content(nullptr)
{
}

/*
 *For use in creating the node data structure
 *
 *@PARAM: If the data is not locally hosted
 *@PARAM: If the node is a directory, not a file
 *@PARAM: The lookup for identifying the node
 *@PARAM: The host of the node
 *@PARAM: The content of the node if it is a file
 */
internal_unbalanced_tree_filesystem::internal_node_data_struct::internal_node_data_struct(bool is_foreign, bool is_directory, string lookup, string host, shared_ptr<char> file_content)
{
	if(!is_foreign)
	{
		this->is_directory = is_directory;
		this->is_foreign = is_foreign;
		this->file_content = file_content;
		this->host = host;
		this->lookup = lookup;
	}
}

/*
 *For use in creating the node data structure
 *
 *@PARAM: If the data is not locally hosted
 *@PARAM: If the node is a directory, not a file
 *@PARAM: The lookup for identifying the node
 *@PARAM: The host of the node
 *@PARAM: The content of the node if it is a file
 */
void internal_unbalanced_tree_filesystem::internal_node_data_struct::set(bool is_foreign, bool is_directory, string lookup, string host, shared_ptr<char> file_content)
{
	if(!is_foreign)
	{
		this->is_directory = is_directory;
		this->is_foreign = is_foreign;
		this->file_content = file_content;
		this->host = host;
		this->lookup = lookup;
	}
}

/*
 *For code compaction.
 *Change the tree's or filesystems head for current
 *operations
 *
 *This allows speeding of operations by giving
 *the benefit of setting the tree for expedited
 *access
 *
 * WARNING: The tree with be updated as though
 * unknown queries belong to a foreign node. This
 * will mark the host as a broadcast address
 *
 *@PARAM: The path to change to
 *@PARAM: If the seek should be set to the current path
 *@RETURN: The total local path completetion
 */
string internal_unbalanced_tree_filesystem::pathChangeInline(string path, bool keep_move)
{
	bool
		path_heaped;

	char
		delimeter;

	int
		levels_down;

	string
		intermediate,
		result;

	istringstream
		iterable_path;

	vector<string>
		delimeter_split_string;

	internal_unbalanced_tree<internal_node_data>::internal_node_container
		child_for_tree_update;

	assert(internal_offset_armed);

	result = "";

	if(path != "")
	{
		delimeter = internal_filesystem_reader.getDelimeter();

		iterable_path.str(path);
		copy(istream_iterator<string>(iterable_path),
		  istream_iterator<string>(),
		  back_inserter(delimeter_split_string));

		auto get_iterator = delimeter_split_string.begin();
		for(	path_heaped = false, levels_down = 0;
				get_iterator != delimeter_split_string.end() && !internal_filesystem_reader.stepForwardDirectory(*get_iterator);
				get_iterator++, ++levels_down)
		{
			if(!path_heaped || !internal_filesystem_tree.moveDown(*get_iterator))
			{
				path_heaped = false;

				child_for_tree_update.element.set(false, true, "");	
				internal_filesystem_tree.addNode(child_for_tree_update);
			}
			else
			{
				result.append(*get_iterator);
				if(levels_down != delimeter_split_string.size() - 2)
					result.push_back(delimeter);
			}
		}

		if(path != result || !keep_move)
		{
			while(levels_down--)
				internal_filesystem_tree.moveUp();
		}

		if(result != "")
			internal_filesystem_reader.changeDirectory("");
	}

	return result;
}

/*
 *Generic initializer
 */
internal_unbalanced_tree_filesystem::internal_unbalanced_tree_filesystem():
	internal_offset_armed(false)
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
internal_unbalanced_tree_filesystem::~internal_unbalanced_tree_filesystem()
{
}

/*
 *Update the offset to the start of the current filesystem
 *
 *Given Unix merely invalidates NULL, to present a uniform
 *expected function, no handling is done. String cannot be NULL
 *therefore this being as internal as it is will have no checking
 *
 * This TEMPORARILY is a one and done, this must be set and once
 * set the object is locked. I intend to update this behavior
 *
 *@PARAM: The offset to the filesystem synchronization point
 *@RETURN: If the offset was able to set or be created
 */
bool internal_unbalanced_tree_filesystem::setFilesystemOffset(string offset)
{
	bool
		result;

	internal_filesystem_offset = offset;
	result = internal_filesystem_reader.setRoot(internal_filesystem_offset);
	internal_offset_armed = result;

	return result;
}

/*
 *Update the known filesystem in the tree 
 *
 *@RETURN: If the representation could be updated
 *				Permissions could trip a bad result
 */
bool internal_unbalanced_tree_filesystem::updateRepresentation()
{
	bool
		write_failed,
		path_change_failed,
		path_failure,
		result;

	int
		last_level;

	long int
		file_size;

	pair<int, string>
		traversal_stack_top;

	shared_ptr<char>
		get_file;

	stack<pair<int, string>>
		traversal_stack;

	assert(internal_offset_armed);

	result = false;
	write_failed = false;
	path_failure = false;
	last_level = 0;

	if(!internal_filesystem_reader.listCurrentDirectory()->empty())
	{
		traversal_stack.push(pair<int, string>(last_level, ""));
		//while(internal_filesystem_tree.moveUp()); //in the event of additional actions on move up
		internal_filesystem_tree.internal_heap_pointer = &internal_filesystem_tree.internal_head;

		while(!traversal_stack.empty())
		{
			traversal_stack_top = traversal_stack.top();
			traversal_stack.pop();

			while(last_level-- > traversal_stack_top.first)
			{
				internal_filesystem_tree.moveUp();
				internal_filesystem_reader.stepBackDirectory();
			}

			path_change_failed = traversal_stack_top.second != pathChangeInline(traversal_stack_top.second, true);
			path_failure |= path_change_failed;

			if(!path_change_failed)
			{
				++last_level;
				for(auto get_iterator : *(internal_filesystem_reader.listCurrentDirectory()))
				{
					if(get_iterator.second)
					{
						get_file = internal_filesystem_reader.getFile(get_iterator.first, &file_size);
						if(get_file == nullptr)
							internal_filesystem_reader.createFile(get_iterator.first, get_file, true, file_size);
						else
							write_failed = true;
					}
					else
						traversal_stack.push(pair<int, string>(last_level, get_iterator.first));
				}
			}
		}

		result = !write_failed & !path_failure;
		
		internal_filesystem_tree.internal_heap_pointer = &internal_filesystem_tree.internal_head;
	}
	else
		result = true;

	return result;
}

/*
 *Serialize a non host file or node request for the corresponding host
 *
 *@PARAM: The lookup id for the item demanded
 *@RETURN: The serialized request for a file or node
 */
string internal_unbalanced_tree_filesystem::serializeRequest(string lookup)
{
	string
		result;

	assert(internal_offset_armed);
	
	result = "";

	return result;
}

/*
 *Deserialize the reponse and update the local filesystem
 *
 *@PARAM: The response from the request's host containing the needed update information
 */
void internal_unbalanced_tree_filesystem::deserializeResponse(string serialized_response)
{
	assert(internal_offset_armed);
}

/*
 *Serialize a response containing the needed information from the requested device
 *
 *@PARAM: The file request for serialization and transfer back
 *@RETURN: The serialized response 
 */
string internal_unbalanced_tree_filesystem::serializeResponse(string serialized_request)
{
	string
		result;

	assert(internal_offset_armed);
	
	result = "";

	return result;
}

/*
 *Get a lookup, or iterate the lookups
 *linearly
 *
 *@PARAM: The item to lookup
 *@RETURN: The item found
 */
string internal_unbalanced_tree_filesystem::getForeignLookup(string lookup)
{
	string
		result;

	assert(internal_offset_armed);

	result = "";

	auto get_iterator = internal_foreign_nodes.find(lookup);
	if(get_iterator != internal_foreign_nodes.end())
		result = get_iterator->second;

	return result;
}

/*
 *Insert a lookup, or iterate the lookups
 *linearly
 *
 *@PARAM: The item path
 *@PARAM: The item host, or broadcast
 */
void internal_unbalanced_tree_filesystem::addForeignLookup(string path, string host)
{
	assert(internal_offset_armed);

	internal_foreign_nodes.emplace(host, path);
}

/*
 *Remove a lookup, or iterate the lookups
 *linearly
 *
 *@PARAM: The item to remove
 *@RETURN: If the item was removed
 */
bool internal_unbalanced_tree_filesystem::removeForeignLookup(string lookup)
{
	bool
		result;

	assert(internal_offset_armed);

	auto get_iterator = internal_foreign_nodes.find(lookup);
	result = (get_iterator != internal_foreign_nodes.end());

	return result;
}

/*
 *Kill the current directory if it is not the top
 *
 *@RETURN: If the parent could be killed
 */
bool internal_unbalanced_tree_filesystem::killParent()
{
	bool
		result;

	shared_ptr<internal_node_data>
		get_node;

	assert(internal_offset_armed);

	result = true;

	get_node = internal_filesystem_tree.getNode();
	
	if(get_node != nullptr)
	{
		if(get_node->is_directory)
		{
			result &= internal_filesystem_reader.removeDirectory();
			result &= internal_filesystem_tree.removeNode();
		}
	}
	else
		result = false;

	return 0;
}

/*
 *Kill the specified child under the current parent
 *directory
 *
 *@PARAM: The child to kill
 *@RETURN: If the child was killed
 */
bool internal_unbalanced_tree_filesystem::killChild(string name)
{
	bool
		result;

	shared_ptr<internal_node_data>
		get_node;

	assert(internal_offset_armed);

	result = true;

	get_node = internal_filesystem_tree.getNode(name);
	
	if(get_node != nullptr)
	{
		if(get_node->is_directory)
		{
			if(stepForwardsOnce(name))
				result = killParent();	
		}
		else
		{
			result &= internal_filesystem_reader.deleteFile(name);
			result &= internal_filesystem_tree.removeNode(name);
		}	
	}
	else
		result = false;

	return 0;
}

/*
 *Verify that a file or directory path exists
 *within the current tree structure. If not
 *update then return that result
 *
 *@PARAM: The path to check
 *@RETURN: If the path exists
 */
bool internal_unbalanced_tree_filesystem::pathExists(string path)
{
	assert(internal_offset_armed);

	return pathChangeInline(path, false) != "";
}

/*
 *Step back in the tree structure, or the file
 *system representation
 *
 *This allows speeding of operations by giving
 *the benefit of setting the tree for expedited
 *access
 *
 *@RETURN: If it moved upwards the filesystem tree structure
 */
bool internal_unbalanced_tree_filesystem::stepBackwardsOnce()
{
	bool
		result;

	assert(internal_offset_armed);

	result = true;

	result &= internal_filesystem_reader.stepBackDirectory();
	result &= internal_filesystem_tree.moveUp(); 

	return result;
}

/*
 *Step forward in the tree structure towards
 *the specified target
 *
 *This allows speeding of operations by giving
 *the benefit of setting the tree for expedited
 *access
 *
 *@PARAM: The child to move down the tree structure to
 *@RETURN: If the system moved downwards
 */
bool internal_unbalanced_tree_filesystem::stepForwardsOnce(string name)
{
	bool
		result;

	assert(internal_offset_armed);

	result = false;

	result &= internal_filesystem_reader.stepForwardDirectory(name);
	result &= internal_filesystem_tree.moveDown(name);

	return result;
}

/*
 *Change the tree's or filesystems head for current
 *operations
 *
 *This allows speeding of operations by giving
 *the benefit of setting the tree for expedited
 *access
 *
 *@PARAM: The path to change to
 *@RETURN: If the path was changed to
 */
bool internal_unbalanced_tree_filesystem::pathChange(string path)
{
	assert(internal_offset_armed);

	return pathChangeInline(path, true) != "";
}

/*
 *Set a unique host identifier
 *This must be unique between the various
 *running clients
 *
 *@PARAM: The unique host identifier string
 */
void internal_unbalanced_tree_filesystem::setHostIdentifier(string identifier)
{
	internal_host_identifier = identifier;
}

/*
 *Clean the data structure for reuse
 */
void internal_unbalanced_tree_filesystem::clean()
{
	assert(internal_offset_armed);

	internal_offset_armed = false;
	internal_filesystem_offset = "";
	internal_host_identifier = "";
	internal_foreign_nodes.clear();

	while(internal_filesystem_tree.moveUp());
	internal_filesystem_tree.clean();
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void internal_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
