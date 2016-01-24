
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __javax_swing_plaf_basic_BasicTreeUI__
#define __javax_swing_plaf_basic_BasicTreeUI__

#pragma interface

#include <javax/swing/plaf/TreeUI.h>
#include <gcj/array.h>

extern "Java"
{
  namespace java
  {
    namespace awt
    {
        class Color;
        class Component;
        class Dimension;
        class Graphics;
        class Insets;
        class Rectangle;
      namespace event
      {
          class ComponentListener;
          class FocusListener;
          class KeyListener;
          class MouseEvent;
          class MouseListener;
      }
    }
    namespace beans
    {
        class PropertyChangeListener;
    }
  }
  namespace javax
  {
    namespace swing
    {
        class ActionMap;
        class CellRendererPane;
        class Icon;
        class JComponent;
        class JTree;
      namespace event
      {
          class CellEditorListener;
          class TreeExpansionListener;
          class TreeModelListener;
          class TreeSelectionListener;
      }
      namespace plaf
      {
          class ComponentUI;
        namespace basic
        {
            class BasicTreeUI;
        }
      }
      namespace tree
      {
          class AbstractLayoutCache;
          class AbstractLayoutCache$NodeDimensions;
          class TreeCellEditor;
          class TreeCellRenderer;
          class TreeModel;
          class TreePath;
          class TreeSelectionModel;
      }
    }
  }
}

class javax::swing::plaf::basic::BasicTreeUI : public ::javax::swing::plaf::TreeUI
{

public:
  BasicTreeUI();
  static ::javax::swing::plaf::ComponentUI * createUI(::javax::swing::JComponent *);
public: // actually protected
  virtual ::java::awt::Color * getHashColor();
  virtual void setHashColor(::java::awt::Color *);
public:
  virtual void setLeftChildIndent(jint);
  virtual jint getLeftChildIndent();
  virtual void setRightChildIndent(jint);
  virtual jint getRightChildIndent();
  virtual void setExpandedIcon(::javax::swing::Icon *);
  virtual ::javax::swing::Icon * getExpandedIcon();
  virtual void setCollapsedIcon(::javax::swing::Icon *);
  virtual ::javax::swing::Icon * getCollapsedIcon();
public: // actually protected
  virtual void setLargeModel(jboolean);
  virtual jboolean isLargeModel();
  virtual void setRowHeight(jint);
  virtual jint getRowHeight();
  virtual void setCellRenderer(::javax::swing::tree::TreeCellRenderer *);
  virtual ::javax::swing::tree::TreeCellRenderer * getCellRenderer();
  virtual void setModel(::javax::swing::tree::TreeModel *);
  virtual ::javax::swing::tree::TreeModel * getModel();
  virtual void setRootVisible(jboolean);
  virtual jboolean isRootVisible();
  virtual void setShowsRootHandles(jboolean);
  virtual jboolean getShowsRootHandles();
  virtual void setCellEditor(::javax::swing::tree::TreeCellEditor *);
  virtual ::javax::swing::tree::TreeCellEditor * getCellEditor();
  virtual void setEditable(jboolean);
  virtual jboolean isEditable();
  virtual void setSelectionModel(::javax::swing::tree::TreeSelectionModel *);
  virtual ::javax::swing::tree::TreeSelectionModel * getSelectionModel();
public:
  virtual ::java::awt::Rectangle * getPathBounds(::javax::swing::JTree *, ::javax::swing::tree::TreePath *);
public: // actually package-private
  virtual jint getMaxHeight(::javax::swing::JTree *);
  virtual ::javax::swing::Icon * getNodeIcon(::javax::swing::tree::TreePath *);
public:
  virtual ::javax::swing::tree::TreePath * getPathForRow(::javax::swing::JTree *, jint);
  virtual jint getRowForPath(::javax::swing::JTree *, ::javax::swing::tree::TreePath *);
  virtual jint getRowCount(::javax::swing::JTree *);
  virtual ::javax::swing::tree::TreePath * getClosestPathForLocation(::javax::swing::JTree *, jint, jint);
  virtual jboolean isEditing(::javax::swing::JTree *);
  virtual jboolean stopEditing(::javax::swing::JTree *);
  virtual void cancelEditing(::javax::swing::JTree *);
  virtual void startEditingAtPath(::javax::swing::JTree *, ::javax::swing::tree::TreePath *);
  virtual ::javax::swing::tree::TreePath * getEditingPath(::javax::swing::JTree *);
public: // actually protected
  virtual void prepareForUIInstall();
  virtual void completeUIInstall();
  virtual void completeUIUninstall();
  virtual void installComponents();
  virtual ::javax::swing::tree::AbstractLayoutCache$NodeDimensions * createNodeDimensions();
  virtual ::java::beans::PropertyChangeListener * createPropertyChangeListener();
  virtual ::java::awt::event::MouseListener * createMouseListener();
  virtual ::java::awt::event::FocusListener * createFocusListener();
  virtual ::java::awt::event::KeyListener * createKeyListener();
  virtual ::java::beans::PropertyChangeListener * createSelectionModelPropertyChangeListener();
  virtual ::javax::swing::event::TreeSelectionListener * createTreeSelectionListener();
  virtual ::javax::swing::event::CellEditorListener * createCellEditorListener();
  virtual ::java::awt::event::ComponentListener * createComponentListener();
  virtual ::javax::swing::event::TreeExpansionListener * createTreeExpansionListener();
  virtual ::javax::swing::tree::AbstractLayoutCache * createLayoutCache();
  virtual ::javax::swing::CellRendererPane * createCellRendererPane();
  virtual ::javax::swing::tree::TreeCellEditor * createDefaultCellEditor();
  virtual ::javax::swing::tree::TreeCellRenderer * createDefaultCellRenderer();
  virtual ::javax::swing::event::TreeModelListener * createTreeModelListener();
  virtual void uninstallListeners();
  virtual void uninstallKeyboardActions();
  virtual void uninstallComponents();
  virtual jint getVerticalLegBuffer();
  virtual jint getHorizontalLegBuffer();
  virtual void updateLayoutCacheExpandedNodes();
  virtual void updateExpandedDescendants(::javax::swing::tree::TreePath *);
  virtual ::javax::swing::tree::TreePath * getLastChildPath(::javax::swing::tree::TreePath *);
  virtual void updateDepthOffset();
  virtual void updateCellEditor();
  virtual void updateRenderer();
  virtual void configureLayoutCache();
  virtual void updateSize();
  virtual void updateCachedPreferredSize();
  virtual void pathWasExpanded(::javax::swing::tree::TreePath *);
  virtual void pathWasCollapsed(::javax::swing::tree::TreePath *);
  virtual void installDefaults();
  virtual void installKeyboardActions();
private:
  ::javax::swing::ActionMap * getActionMap();
  ::javax::swing::ActionMap * createDefaultActions();
  jint convertModifiers(jint);
public: // actually protected
  virtual void installListeners();
public:
  virtual void installUI(::javax::swing::JComponent *);
public: // actually protected
  virtual void uninstallDefaults();
public:
  virtual void uninstallUI(::javax::swing::JComponent *);
  virtual void paint(::java::awt::Graphics *, ::javax::swing::JComponent *);
private:
  jboolean isLastChild(::javax::swing::tree::TreePath *);
public: // actually protected
  virtual void ensureRowsAreVisible(jint, jint);
public:
  virtual void setPreferredMinSize(::java::awt::Dimension *);
  virtual ::java::awt::Dimension * getPreferredMinSize();
  virtual ::java::awt::Dimension * getPreferredSize(::javax::swing::JComponent *);
  virtual ::java::awt::Dimension * getPreferredSize(::javax::swing::JComponent *, jboolean);
  virtual ::java::awt::Dimension * getMinimumSize(::javax::swing::JComponent *);
  virtual ::java::awt::Dimension * getMaximumSize(::javax::swing::JComponent *);
public: // actually protected
  virtual void completeEditing();
  virtual void completeEditing(jboolean, jboolean, jboolean);
  virtual jboolean startEditing(::javax::swing::tree::TreePath *, ::java::awt::event::MouseEvent *);
private:
  void editorRequestFocus(::java::awt::Component *);
public: // actually protected
  virtual void checkForClickInExpandControl(::javax::swing::tree::TreePath *, jint, jint);
  virtual jboolean isLocationInExpandControl(::javax::swing::tree::TreePath *, jint, jint);
  virtual void handleExpandControlClick(::javax::swing::tree::TreePath *, jint, jint);
  virtual void toggleExpandState(::javax::swing::tree::TreePath *);
  virtual jboolean isToggleSelectionEvent(::java::awt::event::MouseEvent *);
  virtual jboolean isMultiSelectEvent(::java::awt::event::MouseEvent *);
  virtual jboolean isToggleEvent(::java::awt::event::MouseEvent *);
  virtual void selectPathForEvent(::javax::swing::tree::TreePath *, ::java::awt::event::MouseEvent *);
  virtual jboolean isLeaf(jint);
public: // actually package-private
  virtual jboolean hasControlIcons();
  virtual ::javax::swing::Icon * getCurrentControlIcon(::javax::swing::tree::TreePath *);
  virtual ::java::lang::Object * getParent(::java::lang::Object *, ::java::lang::Object *);
private:
  ::java::lang::Object * findNode(::java::lang::Object *, ::java::lang::Object *);
public: // actually package-private
  virtual void selectPath(::javax::swing::JTree *, ::javax::swing::tree::TreePath *);
  virtual JArray< ::java::lang::Object * > * getPathToRoot(::java::lang::Object *, jint);
public: // actually protected
  virtual void paintVerticalLine(::java::awt::Graphics *, ::javax::swing::JComponent *, jint, jint, jint);
  virtual void paintHorizontalLine(::java::awt::Graphics *, ::javax::swing::JComponent *, jint, jint, jint);
  virtual void drawCentered(::java::awt::Component *, ::java::awt::Graphics *, ::javax::swing::Icon *, jint, jint);
  virtual void drawDashedHorizontalLine(::java::awt::Graphics *, jint, jint, jint);
  virtual void drawDashedVerticalLine(::java::awt::Graphics *, jint, jint, jint);
  virtual void paintExpandControl(::java::awt::Graphics *, ::java::awt::Rectangle *, ::java::awt::Insets *, ::java::awt::Rectangle *, ::javax::swing::tree::TreePath *, jint, jboolean, jboolean, jboolean);
  virtual void paintHorizontalPartOfLeg(::java::awt::Graphics *, ::java::awt::Rectangle *, ::java::awt::Insets *, ::java::awt::Rectangle *, ::javax::swing::tree::TreePath *, jint, jboolean, jboolean, jboolean);
  virtual void paintVerticalPartOfLeg(::java::awt::Graphics *, ::java::awt::Rectangle *, ::java::awt::Insets *, ::javax::swing::tree::TreePath *);
  virtual void paintRow(::java::awt::Graphics *, ::java::awt::Rectangle *, ::java::awt::Insets *, ::java::awt::Rectangle *, ::javax::swing::tree::TreePath *, jint, jboolean, jboolean, jboolean);
  virtual void prepareForUIUninstall();
  virtual jboolean shouldPaintExpandControl(::javax::swing::tree::TreePath *, jint, jboolean, jboolean, jboolean);
  virtual jint getRowX(jint, jint);
public: // actually package-private
  static jint WAIT_TILL_EDITING;
public: // actually protected
  ::javax::swing::Icon * __attribute__((aligned(__alignof__( ::javax::swing::plaf::TreeUI)))) collapsedIcon;
  ::javax::swing::Icon * expandedIcon;
  jint leftChildIndent;
  jint rightChildIndent;
  jint totalChildIndent;
  jint lastSelectedRow;
  ::javax::swing::JTree * tree;
  ::javax::swing::tree::TreeCellRenderer * currentCellRenderer;
  jboolean createdRenderer;
  ::javax::swing::tree::TreeCellEditor * cellEditor;
  jboolean createdCellEditor;
  jboolean stopEditingInCompleteEditing;
  ::javax::swing::CellRendererPane * rendererPane;
  ::java::awt::Dimension * preferredSize;
  ::java::awt::Dimension * preferredMinSize;
  jboolean validCachedPreferredSize;
  ::javax::swing::tree::AbstractLayoutCache * treeState;
  ::java::util::Hashtable * drawingCache;
  jboolean largeModel;
  ::javax::swing::tree::AbstractLayoutCache$NodeDimensions * nodeDimensions;
  ::javax::swing::tree::TreeModel * treeModel;
  ::javax::swing::tree::TreeSelectionModel * treeSelectionModel;
  jint depthOffset;
  ::java::awt::Component * editingComponent;
  ::javax::swing::tree::TreePath * editingPath;
  jint editingRow;
  jboolean editorHasDifferentSize;
public: // actually package-private
  jboolean isEditing__;
  ::javax::swing::tree::TreePath * currentVisiblePath;
  jint gap;
  jint maxHeight;
  ::java::awt::Color * hashColor;
  ::java::beans::PropertyChangeListener * propertyChangeListener;
  ::java::awt::event::FocusListener * focusListener;
  ::javax::swing::event::TreeSelectionListener * treeSelectionListener;
  ::java::awt::event::MouseListener * mouseListener;
  ::java::awt::event::KeyListener * keyListener;
  ::java::beans::PropertyChangeListener * selectionModelPropertyChangeListener;
  ::java::awt::event::ComponentListener * componentListener;
  ::javax::swing::event::CellEditorListener * cellEditorListener;
  ::javax::swing::event::TreeExpansionListener * treeExpansionListener;
  ::javax::swing::event::TreeModelListener * treeModelListener;
  static ::javax::swing::Icon * nullIcon;
  static ::java::awt::event::MouseEvent * EDIT;
public:
  static ::java::lang::Class class$;
};

#endif // __javax_swing_plaf_basic_BasicTreeUI__
