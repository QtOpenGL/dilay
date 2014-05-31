#include <QVBoxLayout>
#include <QLineEdit>
#include <QDoubleValidator>
#include "view/vector-edit.hpp"

struct ViewVectorEdit::Impl {
  ViewVectorEdit* self;
  glm::vec3       vectorData;
  QLineEdit*      edit [3];

  Impl (ViewVectorEdit* s, const glm::vec3& v) 
    : self       (s) 
    , vectorData (v)
  { 
    QVBoxLayout* layout = new QVBoxLayout;

    this->edit[0] = new QLineEdit;
    this->edit[1] = new QLineEdit;
    this->edit[2] = new QLineEdit;

    QDoubleValidator* validator0 = new QDoubleValidator (this->edit[0]);
    QDoubleValidator* validator1 = new QDoubleValidator (this->edit[1]);
    QDoubleValidator* validator2 = new QDoubleValidator (this->edit[2]);

    this->edit[0]->setValidator (validator0);
    this->edit[1]->setValidator (validator1);
    this->edit[2]->setValidator (validator2);

    this->edit[0]->setText      (validator0->locale ().toString (v.x));
    this->edit[1]->setText      (validator1->locale ().toString (v.y));
    this->edit[2]->setText      (validator2->locale ().toString (v.z));

    layout->setSpacing (0);
    layout->addWidget  (this->edit[0]);
    layout->addWidget  (this->edit[1]);
    layout->addWidget  (this->edit[2]);

    this->self->setLayout (layout);

    QObject::connect (this->edit[0], &QLineEdit::textEdited, [this] (const QString& text)
        { this->componentChanged (0, text); });
    QObject::connect (this->edit[1], &QLineEdit::textEdited, [this] (const QString& text)
        { this->componentChanged (1, text); });
    QObject::connect (this->edit[2], &QLineEdit::textEdited, [this] (const QString& text)
        { this->componentChanged (2, text); });
  };

  void componentChanged (int i, const QString& text) {
    if (this->edit[i]->hasAcceptableInput ()) {
      bool  ok;
      float f = text.toFloat (&ok);
      assert (ok);
      this->vectorData[i] = f;
      emit this->self->vectorEdited (this->vectorData);
    }
  }

  void vector (const glm::vec3& v) { this->x (v.x); this->y (v.y); this->z (v.z); }
  void x      (float v)            { this->changeComponent (0,v); }
  void y      (float v)            { this->changeComponent (1,v); }
  void z      (float v)            { this->changeComponent (2,v); }

  void changeComponent (int i, float v) {
    this->vectorData[i] = v;
    this->edit[i]->setText (std::to_string (v).c_str ());
  }
};

ViewVectorEdit :: ViewVectorEdit (const glm::vec3& v, QWidget* p) : QWidget (p) {
  this->impl = new Impl (this, v);
}

DELEGATE_BIG3_WITHOUT_CONSTRUCTOR (ViewVectorEdit) 

DELEGATE1 (void, ViewVectorEdit, vector, const glm::vec3&)
DELEGATE1 (void, ViewVectorEdit, x     , float)
DELEGATE1 (void, ViewVectorEdit, y     , float)
DELEGATE1 (void, ViewVectorEdit, z     , float)