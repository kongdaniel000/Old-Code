#ifndef SRC_ELECTION_ENTITY_H_
#define SRC_ELECTION_ENTITY_H_

#include <string>

/**
* @brief File name: ElectionEntity.h. Description: Contains method signatures and 
* attributes for the ElectionEntity class, which is a base class of Party. Author: Ruolei*
*/
class ElectionEntity {
public:
    /**
    * @brief Returns the name attribute.
    * 
    * @return The name field.
    **/    
    std::string getName();
    /**
    * @brief Sets the name field.
    * 
    * @param newName the name to be set to.
    **/
    void setName(std::string newName);
//Protected member variable, can be accessed by derived classes.
protected:
    std::string name; //!< the name of Election Entity 
};

#endif 
