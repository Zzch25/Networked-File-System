/*
 *(c) Zachary Job
 *
 *defs_internal.h
 *
 *Generic algorithms and structures reside here
 *For example tree structures
 *
 *Some of these structures will not submit to
 *debugging standards to lower unneeded overhead
 */

#ifndef DEFS_INTERNAL
#define DEFS_INTERNAL

//_DEL_
//Three parameter conflicts are in need of
//resolution. I'll decide after developing
//the tree, handler, and scheduler classes
//
//This is unusual... However I'm
//experimenting for the sake of trying
//something new!
//_DEL_

using namespace std;

/*
 *A structure intended for use in representing
 *the host filesystem and other uses
 */
class internal_unbalanced_tree
{
	private:

		/*
		 struct node, first conflict
		 */

		friend class internal_unbalanced_tree_filesystem;
			
	public:
	
		internal_unbalanced_tree();
		~internal_unbalanced_tree();

		/*
			_25_..._25_..._25_			
			_26_..._26_
			For lazy vi reg update later, and 25 of course 

			_25_addNode_25_()_25_
			_25_removeNode_25_()_25_
			_25_moveUp_25_()_25_
			_25_moveDown_25_()_25_
			_25_clean_25_()_25_
		 */	
};

/*
 *A structure intended for use in representing
 *the host filesystem and other uses
 */
class internal_unbalanced_tree_filesystem
{
	private:
		
	public:
	
		internal_unbalanced_tree_filesystem();
		~internal_unbalanced_tree_filesystem();

		/*
			_25_..._25_..._25_			
			_26_..._26_			
			For lazy vi reg update later, and 25 of course 

			_25_serialize_25_()_25_
			_25_deserialize_25_()_25_
			_25_pathExists_25_()_25_
			_25_stepBackwardsOnce_25_()_25_
			_25_stepForwardsOnce_25_()_25_
			_25_pathChange_25_()_25_
		 */
};

//INTERNAL UNBALANCED TREE///////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 */
internal_unbalanced_tree::internal_unbalanced_tree()
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
internal_unbalanced_tree::~internal_unbalanced_tree()
{
}

/*
 *Add a node to the current parent
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_addNode_26_
{

}
*/

/*
 *Remove a specific node, no input specifies the parent
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_removeNode_26_
{

}
*/

/*
 *Traverse up the tree, or backwards, or slantwise
 *where the vertical vector is upwards biased...
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_moveUp_26_
{

}
*/

/*
 *Traverse downwards on the tree by specifying the
 *desired child. Perhaps this one isn't slantwise
 *at times... I'll stop
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_moveDown_26_
{

}
*/

/*
 *Purge the structure
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_clean_26_
{

}
*/

//INTERNAL UNBALANCED TREE FILESYSTEM////////
/////////////////////////////////////////////

/*
 *Generic initializer
 */
internal_unbalanced_tree_filesystem::internal_unbalanced_tree_filesystem()
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
 *Compact the tree into the most compact representation
 *possible for transport and exhchange of tree data.
 *This is obviously critical when networking.
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_serialize_26_
{

}
*/

/*
 *Load a serialized tree into the structure. This
 *is useful for receiving updated structures
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_deserialize_26_
{

}
*/

/*
 *Verify that a file or directory path exists
 *within the current tree structure
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_pathExists_26_
{

}
*/

/*
 *Step back in the tree structure, or the file
 *system representation
 *
 *This allows speeding of operations by giving
 *the benefit of setting the tree for expedited
 *access
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_stepBackwardsOnce_26_
{

}
*/

/*
 *Step forward in the tree structure towards
 *the specified target
 *
 *This allows speeding of operations by giving
 *the benefit of setting the tree for expedited
 *access
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_stepForwardsOnce_26_
{

}
*/

/*
 *Change the tree's or filesystems head for current
 *operations
 *
 *This allows speeding of operations by giving
 *the benefit of setting the tree for expedited
 *access
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_pathChange_26_
{

}
*/

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void internal_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
