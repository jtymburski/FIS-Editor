#include "Database/EditorCategory.h"

EditorCategory::EditorCategory(QWidget *parent) : QWidget(parent)
{
}

EditorCategory::~EditorCategory()
{
}

EditorCategory* EditorCategory::clone()
{
  return this;
}



