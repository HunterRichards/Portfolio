from pymongo import MongoClient

class AnimalShelter(object):
    """
    CRUD operations for Animal collection in MongoDB.
    """
    
    def __init__(self):
        '''
        Initializing the MongoClient. This helps to 
        access the MongoDB databases and collections.
        This is hard-wired to use the aac database, the 
        animals collection, and the aac user.
        Definitions of the connection string variables are
        unique to the individual Apporto environment.
       
        You must edit the connection variables below to reflect
        your own instance of MongoDB!
        
        Connection Variables
        '''
        
        USER = 'aacuser'
        PASS = 'TestPass4849'
        HOST = 'nv-desktop-services.apporto.com'
        PORT = 30531
        DB = 'AAC'
        COL = 'animals'
        
        #
        # Initialize Connection
        #
        self.client = MongoClient('mongodb://%s:%s@%s:%d' % (USER,PASS,HOST,PORT))
        self.database = self.client['%s' % (DB)]
        self.collection = self.database['%s' % (COL)]

    def create(self, data):
        """
        Insert a new document into the specified collection.
        
        Parameters:
        - data: Dictionary containing the document data to be inserted.
        
        Returns:
        - True if successful insert, False otherwise.
        """
        try:
            if data:
                self.collection.insert_one(data)
                return True
            else:
                return False
        except Exception as e:
            raise Exception(f"Error inserting data into collection: {e}")

    def read(self, query):
        """
        Query documents from the specified collection.
        
        Parameters:
        - query: Dictionary containing the key/value lookup pair for querying. 
                 If query is None or an empty dictionary, all documents are returned.
        
        Returns:
        - List of results if command is successful, else an empty list.
        - If nothing is queried, returns all documents with the "_id" field excluded.
        """
        try:
            return self.collection.find(query,{"_id":False})
        except Exception as e:
            raise Exception(f"Error querying documents from collection: {e}")

    def update(self, query, new_values):
        """
        Update documents in the specified collection based on the provided query.
        
        Parameters:
        - query: Dictionary containing the key/value lookup pair for querying.
        - new_values: Dictionary containing the new key/value pairs to update.
        
        Returns:
        - The number of objects modified in the collection.
        """
        try:
            if query and new_values:
                result = self.collection.update_many(query, {"$set": new_values})
                return result.modified_count
            else:
                return 0
        except Exception as e:
            raise Exception(f"Error updating documents in collection: {e}")

    def delete(self, query):
        """
        Remove documents from the specified collection based on the provided query.
        
        Parameters:
        - query: Dictionary containing the key/value lookup pair for querying.
        
        Returns:
        - The number of objects removed from the collection.
        """
        try:
            if query:
                result = self.collection.delete_many(query)
                return result.deleted_count
            else:
                return 0
        except Exception as e:
            raise Exception(f"Error deleting documents from collection: {e}")