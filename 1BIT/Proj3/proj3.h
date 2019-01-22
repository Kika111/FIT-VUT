
/**
 **@mainpage
 * This program puts objects (points in 2D space) into groups (clusters) based on their distance from each other 
 *
 * @author  Kristína Hostačná <xhosta05@stud.fit.vutbr.cz>
 * @date December 2018
 * @brief Projekt 3 - Simple cluster analysis (Complete linkage)
 * @see http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 * @file proj3.h
 * 
 * @link
 * proj3.h
 * @endlink
 *
*/

/**
 * @struct obj_t
 * @brief      Structure representing object- point in 2D space and its ID and parameters
 */
struct obj_t {
     /** Object identificator (unique for each point) */
    int id;
     /** parameter X- coordinate of object on x axis */
    float x;
     /** parameter Y- coordinate of object on y axis */
    float y;
};

/**
 * @struct cluster_t
 * @brief      Structure representing cluster- group of objects and its attributes
 */
struct cluster_t {
    /**   Number of objects cluster actually contains */
    int size;
    /**   Max number of objects cluster can contain */
    int capacity;
    /**   Pointer to array of objects */
    struct obj_t *obj;
};

/**
 * @defgroup cluster Cluster
 * @brief Functions that work with clusters individualy
 * 
 * @defgroup array_of_clusters Array
 * @brief Functions that work with array of clusters
 * 
 * @defgroup distance Distance
 * @brief Functions that calculate distance
 * 
 * @defgroup output Output
 * @brief Functions that print cluster/clusters
 * 
 * @defgroup constants Constants
 * @brief Constants used
 */

/**
 * @brief      Initialisation of cluster 'c', allocation of memory for 'cap' objects.
 *             If pointer to cluster's array of objects is NULL, 'cap' should be set equal to 0, 
 *
 * @param      c     Pointer to cluster to be initialized
 * @param      cap   Needed capacity - maximum number of objects cluster can hold
 *
 * @return void
 *
 * @pre
 * -# 'c' is not NULL
 * -# 'cap' is not negative
 * 
 * @post
 * -# If allocation has been successful, cluster 'c' has enough memory allocated to be able to contain 'cap' objects 
 *
 * @ingroup cluster
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief      Cleaning cluster by freeing its object array from memory
 *
 * @param      c     Pointer to cluster to be cleaned
 *
 * @return void
 *
 * @pre
 * -# 'c' is not NULL
 * 
 * @post
 * -# Memory allocated for cluster's objects array is freed
 * -# Cluster 'c' is initialised as empty
 *
 * @ingroup cluster
 */
void clear_cluster(struct cluster_t *c);

/**
 * @brief Chunk of cluster objects. Value recommended for reallocation.
 * 
 * @ingroup constants
 */
const int CLUSTER_CHUNK = 10;

/**
 * @brief      Altering cluster capacity
 *
 * @param      c        Pointer to cluster to be resized
 * @param      new_cap  Reqired capacity
 *
 * @return     Returns cluster with required capacity if reallocation was successful, else returns NULL
 *
 * @pre
 * -# 'c' is not NULL 
 * -# 'c->capacity' is not negative
 * -# 'new_cap' is not negative
 * 
 * @post
 * -# Cluster 'c' is resized to 'new_cap' if reallocation was successful
 *
 * @ingroup cluster
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief      Adds an object to the end of cluster, resizing it if needed
 *
 * @param      c     Pointer to cluster to be enlarged
 * @param      obj   Object to be added to the cluster
 *
 * @return void
 *
 * @pre
 * -# 'c' is not NULL
 * 
 * @post
 * -# Object is added to the end of cluster unless reallocation was needed and unsuccessful
 *
 * @ingroup cluster
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief      Sorts objects in cluster in ascending order based on their IDs
 *
 * @param      c     Pointer to cluster to be sorted
 *
 * @return void
 *
 * @pre
 * -# 'c' is not NULL
 * 
 * @post
 * -# Objects of cluster 'c' are sorted in memory in ascending order based on their IDs
 *
 * @ingroup cluster
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief      Adds all objects from cluster 'c2' to cluster 'c1' resizing it if needed and sorts its objects by id afterwards.
 *             Cluster 'c2' is not changed.
 *
 * @param      c1    Pointer to the target cluster
 * @param      c2    Pointer to the source cluster
 *
 * @return void
 *
 * @pre
 * -# 'c1' and 'c2' are not NULL
 * 
 * @post
 * -# Cluster 'c1' contains all its objects and all objects of cluster 'c2'
 * -# Cluster 'c1' was resized unless reallocation was unsuccessful
 * -# Objects of 'c1' are sorted in ascending order by their IDs
 *
 * @ingroup cluster
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);


/**
 * @brief      Removes cluster with index 'idx' from cluster array 'carr' and clears that cluster.
 *
 * @param      carr  Pointer to the array of clusters
 * @param      narr  Current amount of clusters in array
 * @param      idx   Index of cluster to be removed from array
 *
 * @return     New number of clusters in array
 *
 * @pre
 * -# 'carr' is not NULL
 * -# 'narr' is positive
 * -# 'idx' is less than 'narr'
 * 
 * @post
 * -# Removed cluster is cleared and memory used by its object array is freed
 * -# Remaining clusters inside clusters array are shifted appropriately
 *
 * @ingroup array_of_clusters
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);


/**
 * @brief      Returns object distance calculated with Euclidean_distance.
 *
 * @see https://en.wikipedia.org/wiki/Euclidean_distance
 * 
 * @param      o1    Pointer to the first object
 * @param      o2    Pointer to the second object
 *
 * @return     Decimal distance between objects 'o1' and 'o2' 
 *
 * @pre
 * -# 'o1' and 'o2' are not NULL
 * -# 'o1' and 'o2' are not empty
 *
 * @ingroup distance
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief      Returns cluster distance based on single linkage method.
 *
 * @param      c1    Pointer to the first cluster
 * @param      c2    Pointer to the second cluster
 *
 * @return     Decimal distance between clusters 'c1' and 'c2'
 *
 * @pre
 * -# 'c1' and 'c2' are not NULL
 * -# 'c1' and 'c2' are not empty
 *
 * @ingroup distance
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief      Finds the two closest clusters and saves their indexes in cluster array.
 *
 * @param           carr  Pointer to the cluster array
 * @param           narr  Current amount of clusters in cluster array
 * @param           c1    Pointer to the integer to save first cluster's index into
 * @param           c2    Pointer to the integer to save second cluster's index into
 *
 * @return void
 *
 * @pre
 * -# 'carr' is not NULL
 * -# 'narr' is positive
 * 
 * @post
 * -# If 'c1' or 'c2' are not NULL, found cluster's indexes are saved there
 *
 * @ingroup distance
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief      Prints cluster to standard output.
 *
 * @param      c     Pointer to the actual cluster
 *
 * @return void
 *
 * @pre
 * -# 'c' is not NULL
 * @post
 * -# Brief informations of cluster 'c' are directed to standard output
 *
 * @ingroup output
 */
void print_cluster(struct cluster_t *c);


/**
 * @brief      Reads a file, loads objects and creates clusters for them,
 *               allocating memory needed for cluster array and adding clusters to it.
 *
 * @param      filename  The name of file to read objects from
 * @param      arr       Pointer to the adress of memory holding cluster array to write into
 *
 * @return     Amount of objects properly read and loaded
 *
 * @pre
 * -# File specified in 'filename' exists
 * -# First line of file 'filename' has format "count=N"
 * -# Other lines of file 'filename' have format "ID X Y" where:
 * -# ID is number
 * -# x= <0,1000>
 * -# Y= <0,1000>
 * -# 'arr' is not NULL
 * 
 * @post
 * -# Properly sized cluster array is made
 * -# All objects all loaded and their clusters are saved in cluster array
 *
 * @ingroup array_of_clusters
 */
int load_clusters(char *filename, struct cluster_t **arr);


/**
 * @brief      Prints 'narr' of clusters from cluster array to standard output.
 *
 * @param      carr  Pointer to the cluster array
 * @param      narr  Amount of clusters to be print
 *
 * @return void
 *
 * @pre
 * -# 'carr' is not NULL
 * 
 * @post
 * -# Clusters (and their objects) from 'narr' printed to standard output
 *
 * @ingroup output
 */
void print_clusters(struct cluster_t *carr, int narr);

