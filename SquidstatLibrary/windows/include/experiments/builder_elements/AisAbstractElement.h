#ifndef SQUIDSTATLIBRARY_AISABSTRACTELEMENT_H
#define SQUIDSTATLIBRARY_AISABSTRACTELEMENT_H

#include <QString>
#include "AisSquidstatGlobal.h"
#include <memory>

class AbstractBuilderElement;

/**
 * @brief This is an abstract class that has common code for other elements to inherent from.
 * @note This class is not meant for stand-alone use.
*/
class SQUIDSTATLIBRARY_EXPORT AisAbstractElement {
public:
    virtual ~AisAbstractElement();

    /**
     * @brief An abstract function to give back the name of the element.
     * @return The name of the element.
     * @note This is a pure virtual (abstract) function. It has no implementation in this class and, it must be implemented in any derived class.
    */
    virtual QString getName() const = 0;

    /**
     * @brief An abstract function to give back the applicable categories of the element.
     * @return The name of the experiment.
     * @note This is a pure virtual (abstract) function. It has no implementation in this class and, it must be implemented in any derived class.
    */
    virtual QStringList getCategory() const = 0;

protected:
    std::shared_ptr<AbstractBuilderElement> m_data;

    
    friend class AisExperiment;
};


#endif //SQUIDSTATLIBRARY_AISABSTRACTELEMENT_H
