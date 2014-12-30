#include "Database/EditorBubby.h"

EditorBubby::EditorBubby(QWidget *parent) : QWidget(parent)
{
}

EditorBubby::~EditorBubby()
{
}

EditorBubby* EditorBubby::clone()
{
  return this;
}


