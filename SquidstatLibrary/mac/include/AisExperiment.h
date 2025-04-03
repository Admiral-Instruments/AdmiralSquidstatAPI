#ifndef SQUIDSTATLIBRARY_AISCUSTOMEXPERIMENTRUNNER_H
#define SQUIDSTATLIBRARY_AISCUSTOMEXPERIMENTRUNNER_H

#include "AisSquidstatGlobal.h"
#include "experiments/builder_elements/AisAbstractElement.h"
#include <QString>

class CustomExperimentRunner;
class AisExperimentPrivate;

/**
 *
 * @ingroup InstrumentControl
 * 
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
     * @brief Append an element to this experiment.
     * @param element The experiment element to be appended to this experiment.
     * @param repeat The number of times this element will be repeated. This is an optional parameter with a default value of 1.\n
                     The minimum value is 1. If smaller the function <u>will not append</u> the node to the experiment.\n
                     The maximum value is 65535. The function will clamp any value greater than this to 65535, and it <u>will append</u> the node to the experiment.
     * @return <b>true</b> if the element was appended to the experiment and <b>false</b> otherwise.
     * @note Although an element is an experiment, in the context of custom experiments, it is referred to as an element to make a distinction between the two.
     * In other contexts where such distinction is not needed, an element may still be referred to as an experiment.
    */
    bool appendElement(AisAbstractElement& element, unsigned int repeat = 1);

    /**
     * @brief Append a sub experiment to this experiment.
     * @param subExp A sub experiment to be appended to this experiment.
     * @param repeat The number of times this sub experiment will be repeated. This is an optional parameter with a default value of 1.\n
                     The minimum value is 1. If smaller, the function <u>will not append</u> the sub experiment to the experiment\n
                     The maximum value is 65535. The function will clamp any value greater than this to 65535, and it <u>will append</u> the <b>sub experiment</b> to the experiment.
     * @return <b>true</b> if the sub experiment was appended to the experiment and <b>false</b> otherwise.
    */
    bool appendSubExperiment(const AisExperiment& subExp, unsigned int repeat = 1);

private:
    friend class AisInstrumentHandler;
    std::shared_ptr<AisExperimentPrivate> m_data;
};

#endif //SQUIDSTATLIBRARY_AISCUSTOMEXPERIMENTRUNNER_H