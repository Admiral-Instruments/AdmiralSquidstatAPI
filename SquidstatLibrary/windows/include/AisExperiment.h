#ifndef SQUIDSTATLIBRARY_AISCUSTOMEXPERIMENTRUNNER_H
#define SQUIDSTATLIBRARY_AISCUSTOMEXPERIMENTRUNNER_H

#include "AisSquidstatGlobal.h"
#include "experiments/builder_elements/AisAbstractElement.h"
#include <QString>

class CustomExperimentRunner;
class AisExperimentPrivate;

/**
 * @brief this class is used to create custom experiments. A custom experiment contains one or more elements.
 * Once you create elements and set their parameters, you can add them to the container.
 *
 * @note we call the basic experiments -that are used to build more complex custom experiments- elements.
 * In contexts where both elements and custom experiments are used, elements will be referred to as elements to make the distinction.
 * In other contexts, elements may also be referred to as experiments as they may indeed be used as experiments.
*/
class SQUIDSTATLIBRARY_EXPORT AisExperiment final {
public:
    /**
     * @brief this is the default constructor for the custom experiment.
    */
    explicit AisExperiment();

    /**
     * @brief this is the copy constructor for the custom experiment.
     * @param exp the custom experiment to copy from.
    */
    explicit AisExperiment(const AisExperiment& exp);

    /**
     * @brief the assignment operator for the custom experiment.
     * @param exp the custom experiment to copy from.
    */
    void operator=(const AisExperiment& exp);

    ~AisExperiment();

    /**
     * @brief get the name of the custom experiment.
     * @return the name set for the custom experiment.
     * If no name has been set, the default name returned is "Custom Experiment"
    */
    QString getExperimentName() const;

    /**
     * @brief get a brief description of the custom experiment.
     * @return the description set for the custom experiment.
     * If no description has been set, the default description returned is "Not Defined".
    */
    QString getDescription() const;

    /**
     * @brief get the category for the custom experiment.
     * @return the category set for the custom experiment.
     * If no category has been set, the default category returned is ("Custom").
    */
    QStringList getCategory() const;

    /**
     * @brief set a name for the custom experiment.
     * @param name the name to be set for the custom experiment.
    */
    void setExperimentName(QString name);

    /**
     * @brief set a description for the experiment.
     * @param description the description to be set for the custom experiment.
    */
    void setDescription(QString description);

    /**
     * @brief append an element to the custom experiment.
     * @param element an elemental experiment to be appended to this/(the calling) custom experiment.
     * @param repeat the number of times this element is to be repeated. This is an optional parameter and is defaulted to equal 1 when not set.
     * @return true if appending the element was successful and false otherwise.
     * @note although an element is an experiment, in the context of custom experiments, it is referred to as an element to make a distinction between the two.
     * In other contexts where such distinction is not needed, an element may still be referred to as an experiment.
    */
    bool appendElement(AisAbstractElement& element, uint repeat = 1);

    /**
     * @brief append a sub experiment to this/(the calling) custom experiment.
     * @param subExp a sub experiment to be appended to this/(the calling) custom experiment.
     * @param repeat the number of times this sub experiment is to be repeated. This is an optional parameter and is defaulted to equal 1 when not set.
     * @return true if appending the sub experiment was successful and false otherwise.
    */
    bool appendSubExperiment(const AisExperiment& subExp, uint repeat = 1);

private:
    friend class AisInstrumentHandler;
    std::shared_ptr<AisExperimentPrivate> m_data;
};

#endif //SQUIDSTATLIBRARY_AISCUSTOMEXPERIMENTRUNNER_H