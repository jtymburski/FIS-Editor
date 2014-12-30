#include "Database/EditorAction.h"

EditorAction::EditorAction(QWidget *parent) : QWidget(parent)
{
}

EditorAction::~EditorAction()
{
}

EditorAction* EditorAction::clone()
{
  return this;
}

