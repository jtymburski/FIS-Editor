/*******************************************************************************
 * Class Name: EditorParty
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Party
 ******************************************************************************/
#include "Database/EditorParty.h"

EditorParty::EditorParty(QWidget *parent) : QWidget(parent)
{
}

EditorParty::~EditorParty()
{
}

EditorParty* EditorParty::clone()
{
  return this;
}

