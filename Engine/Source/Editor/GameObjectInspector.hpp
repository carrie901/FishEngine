#ifndef GAMEOBJECTINSPECTOR_HPP
#define GAMEOBJECTINSPECTOR_HPP

#include <QWidget>

namespace Ui {
class GameObjectInspector;
}

class GameObjectInspector : public QWidget
{
    Q_OBJECT

public:
    explicit GameObjectInspector(QWidget *parent = 0);
    ~GameObjectInspector();

public Q_SLOTS:
    void UpdateInspector();

    void OnNameChanged();
    void OnActiveCheckBoxChanged(bool);
    void OnLayerChanged(int);
    void OnTagChanged(int);

private:
    Ui::GameObjectInspector *ui;

    bool m_changed      = false;

    bool m_isActive     = true;
    bool m_isStatic     = false;
    std::string m_name;

    int m_layerIndex    = -1;
    int m_layerDirtyFlag = -1;

    int m_tagIndex      = -1;
    int m_tagDirtyFlag  = -1;
};

#endif // GAMEOBJECTINSPECTOR_HPP
