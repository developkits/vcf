###########################################################
#
#CVS Log info
#$Log$
#Revision 1.2  2003/02/26 04:30:25  ddiego
#merge of code in the devmain-0-5-9 branch into the current tree.
#most additions are in the area of the current linux port, but the major
#addition to this release is the addition of a Condition class (currently
#still under development) and the change over to using the Delegate class
#exclusively from the older event handler macros.
#
#Revision 1.1.2.3  2003/02/08 17:55:44  ddiego
#modified the makefiles to have cvs Log entries, so we can keepo track of
#whats going on :)
#
#
#
#
#Object files for the ApplicationKit 
#
###########################################################



###########################################################
#Debug files
###########################################################

$(OUTDIR_AK_D)/ApplicationKit.o : $(SRC_CORE)/ApplicationKit.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ApplicationKit.cpp -o $(OUTDIR_AK_D)/ApplicationKit.o


$(OUTDIR_AK_D)/AbstractApplication.o : $(SRC_CORE)/AbstractApplication.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractApplication.cpp -o $(OUTDIR_AK_D)/AbstractApplication.o

$(OUTDIR_AK_D)/AbstractCommand.o : $(SRC_CORE)/AbstractCommand.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractCommand.cpp -o $(OUTDIR_AK_D)/AbstractCommand.o

$(OUTDIR_AK_D)/AbstractComponentEditor.o : $(SRC_CORE)/AbstractComponentEditor.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractComponentEditor.cpp -o $(OUTDIR_AK_D)/AbstractComponentEditor.o

$(OUTDIR_AK_D)/AbstractContainer.o : $(SRC_CORE)/AbstractContainer.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractContainer.cpp -o $(OUTDIR_AK_D)/AbstractContainer.o

$(OUTDIR_AK_D)/AbstractListModel.o : $(SRC_CORE)/AbstractListModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractListModel.cpp -o $(OUTDIR_AK_D)/AbstractListModel.o

$(OUTDIR_AK_D)/AbstractPropertyEditor.o : $(SRC_CORE)/AbstractPropertyEditor.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractPropertyEditor.cpp -o $(OUTDIR_AK_D)/AbstractPropertyEditor.o

$(OUTDIR_AK_D)/AbstractScrollable.o : $(SRC_CORE)/AbstractScrollable.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractScrollable.cpp -o $(OUTDIR_AK_D)/AbstractScrollable.o

$(OUTDIR_AK_D)/AbstractTableModel.o : $(SRC_CORE)/AbstractTableModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractTableModelcpp -o $(OUTDIR_AK_D)/AbstractTableModel.o

$(OUTDIR_AK_D)/AbstractTextModel.o : $(SRC_CORE)/AbstractTextModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractTextModel.cpp -o $(OUTDIR_AK_D)/AbstractTextModel.o

$(OUTDIR_AK_D)/AbstractTreeModel.o : $(SRC_CORE)/AbstractTreeModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractTreeModel.cpp -o $(OUTDIR_AK_D)/AbstractTreeModel.o

$(OUTDIR_AK_D)/AbstractView.o : $(SRC_CORE)/AbstractView.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AbstractView.cpp -o $(OUTDIR_AK_D)/AbstractView.o

$(OUTDIR_AK_D)/AcceleratorKey.o : $(SRC_CORE)/AcceleratorKey.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/AcceleratorKey.cpp -o $(OUTDIR_AK_D)/AcceleratorKey.o

$(OUTDIR_AK_D)/Application.o : $(SRC_CORE)/Application.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Application.cpp -o $(OUTDIR_AK_D)/Application.o

$(OUTDIR_AK_D)/Basic3DBorder.o : $(SRC_GRF)/Basic3DBorder.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_GRF)/Basic3DBorder.cpp -o $(OUTDIR_AK_D)/Basic3DBorder.o

$(OUTDIR_AK_D)/BasicTableItemEditor.o : $(SRC_CORE)/BasicTableItemEditor.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/BasicTableItemEditor.cpp -o $(OUTDIR_AK_D)/BasicTableItemEditor.o

$(OUTDIR_AK_D)/ButtonEvent.o : $(SRC_EVENT)/ButtonEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ButtonEvent.cpp -o $(OUTDIR_AK_D)/ButtonEvent.o

$(OUTDIR_AK_D)/CheckBoxControl.o : $(SRC_CORE)/CheckBoxControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CheckBoxControl.cpp -o $(OUTDIR_AK_D)/CheckBoxControl.o

$(OUTDIR_AK_D)/Clipboard.o : $(SRC_CORE)/Clipboard.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Clipboard.cpp -o $(OUTDIR_AK_D)/Clipboard.o

$(OUTDIR_AK_D)/ClipboardDataObject.o : $(SRC_CORE)/ClipboardDataObject.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ClipboardDataObject.cpp -o $(OUTDIR_AK_D)/ClipboardDataObject.o

$(OUTDIR_AK_D)/ClipboardEvent.o : $(SRC_EVENT)/ClipboardEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ClipboardEvent.cpp -o $(OUTDIR_AK_D)/ClipboardEvent.o

$(OUTDIR_AK_D)/ColumnItemEvent.o : $(SRC_EVENT)/ColumnItemEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ColumnItemEvent.cpp -o $(OUTDIR_AK_D)/ColumnItemEvent.o

$(OUTDIR_AK_D)/ColumnModelEvent.o : $(SRC_EVENT)/ColumnModelEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ColumnModelEvent.cpp -o $(OUTDIR_AK_D)/ColumnModelEvent.o

$(OUTDIR_AK_D)/ComboBoxControl.o : $(SRC_CORE)/ComboBoxControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ComboBoxControl.cpp -o $(OUTDIR_AK_D)/ComboBoxControl.o

$(OUTDIR_AK_D)/CommandButton.o : $(SRC_CORE)/CommandButton.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CommandButton.cpp -o $(OUTDIR_AK_D)/CommandButton.o

$(OUTDIR_AK_D)/CommandGroup.o : $(SRC_CORE)/CommandGroup.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CommandGroup.cpp -o $(OUTDIR_AK_D)/CommandGroup.o

$(OUTDIR_AK_D)/CommonColor.o : $(SRC_CORE)/CommonColor.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CommonColor.cpp -o $(OUTDIR_AK_D)/CommonColor.o

$(OUTDIR_AK_D)/CommonFileBrowse.o : $(SRC_CORE)/CommonFileBrowse.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CommonFileBrowse.cpp -o $(OUTDIR_AK_D)/CommonFileBrowse.o

$(OUTDIR_AK_D)/CommonFileDialog.o : $(SRC_CORE)/CommonFileDialog.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CommonFileDialog.cpp -o $(OUTDIR_AK_D)/CommonFileDialog.o

$(OUTDIR_AK_D)/CommonFileOpen.o : $(SRC_CORE)/CommonFileOpen.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CommonFileOpen.cpp -o $(OUTDIR_AK_D)/CommonFileOpen.o

$(OUTDIR_AK_D)/CommonFileSave.o : $(SRC_CORE)/CommonFileSave.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CommonFileSave.cpp -o $(OUTDIR_AK_D)/CommonFileSave.o

$(OUTDIR_AK_D)/CommonFont.o : $(SRC_CORE)/CommonFont.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CommonFont.cpp -o $(OUTDIR_AK_D)/CommonFont.o

$(OUTDIR_AK_D)/CommonPrint.o : $(SRC_CORE)/CommonPrint.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CommonPrint.cpp -o $(OUTDIR_AK_D)/CommonPrint.o

$(OUTDIR_AK_D)/Component.o : $(SRC_CORE)/Component.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Component.cpp -o $(OUTDIR_AK_D)/Component.o

$(OUTDIR_AK_D)/ComponentEditorManager.o : $(SRC_CORE)/ComponentEditorManager.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ComponentEditorManager.cpp -o $(OUTDIR_AK_D)/ComponentEditorManager.o

$(OUTDIR_AK_D)/ComponentInfo.o : $(SRC_CORE)/ComponentInfo.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ComponentInfo.cpp -o $(OUTDIR_AK_D)/ComponentInfo.o

$(OUTDIR_AK_D)/Control.o : $(SRC_CORE)/Control.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Control.cpp -o $(OUTDIR_AK_D)/Control.o

$(OUTDIR_AK_D)/ControlContainer.o : $(SRC_CORE)/ControlContainer.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ControlContainer.cpp -o $(OUTDIR_AK_D)/ControlContainer.o

$(OUTDIR_AK_D)/ControlEvent.o : $(SRC_EVENT)/ControlEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ControlEvent.cpp -o $(OUTDIR_AK_D)/ControlEvent.o

$(OUTDIR_AK_D)/ControlGraphicsContext.o : $(SRC_GRF)/ControlGraphicsContext.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_GRF)/ControlGraphicsContext.cpp -o $(OUTDIR_AK_D)/ControlGraphicsContext.o

$(OUTDIR_AK_D)/Cursor.o : $(SRC_CORE)/Cursor.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Cursor.cpp -o $(OUTDIR_AK_D)/Cursor.o

$(OUTDIR_AK_D)/CursorManager.o : $(SRC_CORE)/CursorManager.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CursorManager.cpp -o $(OUTDIR_AK_D)/CursorManager.o

$(OUTDIR_AK_D)/CustomControl.o : $(SRC_CORE)/CustomControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/CustomControl.cpp -o $(OUTDIR_AK_D)/CustomControl.o

$(OUTDIR_AK_D)/DataType.o : $(SRC_CORE)/DataType.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DataType.cpp -o $(OUTDIR_AK_D)/DataType.o

$(OUTDIR_AK_D)/DefaultColumnItem.o : $(SRC_CORE)/DefaultColumnItem.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultColumnItem.cpp -o $(OUTDIR_AK_D)/DefaultColumnItem.o

$(OUTDIR_AK_D)/DefaultColumnModel.o : $(SRC_CORE)/DefaultColumnModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultColumnModel.cpp -o $(OUTDIR_AK_D)/DefaultColumnModel.o

$(OUTDIR_AK_D)/DefaultListItem.o : $(SRC_CORE)/DefaultListItem.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultListItem.cpp -o $(OUTDIR_AK_D)/DefaultListItem.o

$(OUTDIR_AK_D)/DefaultListModel.o : $(SRC_CORE)/DefaultListModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultListModel.cpp -o $(OUTDIR_AK_D)/DefaultListModel.o

$(OUTDIR_AK_D)/DefaultMenuItem.o : $(SRC_CORE)/DefaultMenuItem.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultMenuItem.cpp -o $(OUTDIR_AK_D)/DefaultMenuItem.o

$(OUTDIR_AK_D)/DefaultPropertyEditors.o : $(SRC_CORE)/DefaultPropertyEditors.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultPropertyEditors.cpp -o $(OUTDIR_AK_D)/DefaultPropertyEditors.o

$(OUTDIR_AK_D)/DefaultTableCellItem.o : $(SRC_CORE)/DefaultTableCellItem.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultTableCellItem.cpp -o $(OUTDIR_AK_D)/DefaultTableCellItem.o

$(OUTDIR_AK_D)/DefaultTableModel.o : $(SRC_CORE)/DefaultTableModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultTableModel.cpp -o $(OUTDIR_AK_D)/DefaultTableModel.o

$(OUTDIR_AK_D)/DefaultTabModel.o : $(SRC_CORE)/DefaultTabModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultTabModel.cpp -o $(OUTDIR_AK_D)/DefaultTabModel.o

$(OUTDIR_AK_D)/DefaultTabPage.o : $(SRC_CORE)/DefaultTabPage.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultTabPage.cpp -o $(OUTDIR_AK_D)/DefaultTabPage.o

$(OUTDIR_AK_D)/DefaultTextModel.o : $(SRC_CORE)/DefaultTextModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultTextModel.cpp -o $(OUTDIR_AK_D)/DefaultTextModel.o

$(OUTDIR_AK_D)/DefaultTreeItem.o : $(SRC_CORE)/DefaultTreeItem.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultTreeItem.cpp -o $(OUTDIR_AK_D)/DefaultTreeItem.o

$(OUTDIR_AK_D)/DefaultTreeModel.o : $(SRC_CORE)/DefaultTreeModel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/DefaultTreeModel.cpp -o $(OUTDIR_AK_D)/DefaultTreeModel.o

$(OUTDIR_AK_D)/Desktop.o : $(SRC_CORE)/Desktop.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Desktop.cpp -o $(OUTDIR_AK_D)/Desktop.o

$(OUTDIR_AK_D)/Dialog.o : $(SRC_CORE)/Dialog.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Dialog.cpp -o $(OUTDIR_AK_D)/Dialog.o

$(OUTDIR_AK_D)/DragEvent.o : $(SRC_EVENT)/DragEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/DragEvent.cpp -o $(OUTDIR_AK_D)/DragEvent.o

$(OUTDIR_AK_D)/DragSource.o : $(SRC_DND)/DragSource.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_DND)/DragSource.cpp -o $(OUTDIR_AK_D)/DragSource.o

$(OUTDIR_AK_D)/DropEvent.o : $(SRC_EVENT)/DropEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/DropEvent.cpp -o $(OUTDIR_AK_D)/DropEvent.o

$(OUTDIR_AK_D)/DropTarget.o : $(SRC_DND)/DropTarget.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_DND)/DropTarget.cpp -o $(OUTDIR_AK_D)/DropTarget.o

$(OUTDIR_AK_D)/FocusEvent.o : $(SRC_EVENT)/FocusEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/FocusEvent.cpp -o $(OUTDIR_AK_D)/FocusEvent.o

$(OUTDIR_AK_D)/Frame.o : $(SRC_CORE)/Frame.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Frame.cpp -o $(OUTDIR_AK_D)/Frame.o

$(OUTDIR_AK_D)/HeaderControl.o : $(SRC_CORE)/HeaderControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/HeaderControl.cpp -o $(OUTDIR_AK_D)/HeaderControl.o

$(OUTDIR_AK_D)/HelpEvent.o : $(SRC_EVENT)/HelpEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/HelpEvent.cpp -o $(OUTDIR_AK_D)/HelpEvent.o

$(OUTDIR_AK_D)/HTMLBrowserControl.o : $(SRC_CORE)/HTMLBrowserControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/HTMLBrowserControl.cpp -o $(OUTDIR_AK_D)/HTMLBrowserControl.o

$(OUTDIR_AK_D)/ImageControl.o : $(SRC_CORE)/ImageControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ImageControl.cpp -o $(OUTDIR_AK_D)/ImageControl.o

$(OUTDIR_AK_D)/ImageList.o : $(SRC_GRF)/ImageList.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_GRF)/ImageList.cpp -o $(OUTDIR_AK_D)/ImageList.o

$(OUTDIR_AK_D)/ImageListEvent.o : $(SRC_EVENT)/ImageListEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ImageListEvent.cpp -o $(OUTDIR_AK_D)/ImageListEvent.o

$(OUTDIR_AK_D)/ItemEvent.o : $(SRC_EVENT)/ItemEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ItemEvent.cpp -o $(OUTDIR_AK_D)/ItemEvent.o

$(OUTDIR_AK_D)/KeyboardEvent.o : $(SRC_EVENT)/KeyboardEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/KeyboardEvent.cpp -o $(OUTDIR_AK_D)/KeyboardEvent.o

$(OUTDIR_AK_D)/Label.o : $(SRC_CORE)/Label.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Label.cpp -o $(OUTDIR_AK_D)/Label.o

$(OUTDIR_AK_D)/LibraryApplication.o : $(SRC_CORE)/LibraryApplication.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/LibraryApplication.cpp -o $(OUTDIR_AK_D)/LibraryApplication.o

$(OUTDIR_AK_D)/Light3DBorder.o : $(SRC_GRF)/Light3DBorder.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_GRF)/Light3DBorder.cpp -o $(OUTDIR_AK_D)/Light3DBorder.o

$(OUTDIR_AK_D)/LightweightComponent.o : $(SRC_CORE)/LightweightComponent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/LightweightComponent.cpp -o $(OUTDIR_AK_D)/LightweightComponent.o

$(OUTDIR_AK_D)/ListBoxControl.o : $(SRC_CORE)/ListBoxControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ListBoxControl.cpp -o $(OUTDIR_AK_D)/ListBoxControl.o

$(OUTDIR_AK_D)/ListModelEvent.o : $(SRC_EVENT)/ListModelEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ListModelEvent.cpp -o $(OUTDIR_AK_D)/ListModelEvent.o

$(OUTDIR_AK_D)/ListViewControl.o : $(SRC_CORE)/ListViewControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ListViewControl.cpp -o $(OUTDIR_AK_D)/ListViewControl.o

$(OUTDIR_AK_D)/Menu.o : $(SRC_CORE)/Menu.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Menu.cpp -o $(OUTDIR_AK_D)/Menu.o

$(OUTDIR_AK_D)/MenuBar.o : $(SRC_CORE)/MenuBar.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/MenuBar.cpp -o $(OUTDIR_AK_D)/MenuBar.o

$(OUTDIR_AK_D)/MenuItemEvent.o : $(SRC_EVENT)/MenuItemEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/MenuItemEvent.cpp -o $(OUTDIR_AK_D)/MenuItemEvent.o

$(OUTDIR_AK_D)/Model.o : $(SRC_CORE)/Model.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Model.cpp -o $(OUTDIR_AK_D)/Model.o

$(OUTDIR_AK_D)/MouseEvent.o : $(SRC_EVENT)/MouseEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/MouseEvent.cpp -o $(OUTDIR_AK_D)/MouseEvent.o

$(OUTDIR_AK_D)/MultilineTextControl.o : $(SRC_CORE)/MultilineTextControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/MultilineTextControl.cpp -o $(OUTDIR_AK_D)/MultilineTextControl.o

$(OUTDIR_AK_D)/Panel.o : $(SRC_CORE)/Panel.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Panel.cpp -o $(OUTDIR_AK_D)/Panel.o

$(OUTDIR_AK_D)/PopupMenu.o : $(SRC_CORE)/PopupMenu.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/PopupMenu.cpp -o $(OUTDIR_AK_D)/PopupMenu.o

$(OUTDIR_AK_D)/PropertyEditorManager.o : $(SRC_CORE)/PropertyEditorManager.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/PropertyEditorManager.cpp -o $(OUTDIR_AK_D)/PropertyEditorManager.o

$(OUTDIR_AK_D)/PushButton.o : $(SRC_CORE)/PushButton.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/PushButton.cpp -o $(OUTDIR_AK_D)/PushButton.o

$(OUTDIR_AK_D)/RadioButtonControl.o : $(SRC_CORE)/RadioButtonControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/RadioButtonControl.cpp -o $(OUTDIR_AK_D)/RadioButtonControl.o

$(OUTDIR_AK_D)/Resource.o : $(SRC_UTILS)/Resource.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_UTILS)/Resource.cpp -o $(OUTDIR_AK_D)/Resource.o

$(OUTDIR_AK_D)/ResourceStream.o : $(SRC_IO)/ResourceStream.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IO)/ResourceStream.cpp -o $(OUTDIR_AK_D)/ResourceStream.o

$(OUTDIR_AK_D)/ScrollbarManager.o : $(SRC_CORE)/ScrollbarManager.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ScrollbarManager.cpp -o $(OUTDIR_AK_D)/ScrollbarManager.o

$(OUTDIR_AK_D)/Splitter.o : $(SRC_CORE)/Splitter.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Splitter.cpp -o $(OUTDIR_AK_D)/Splitter.o

$(OUTDIR_AK_D)/StatusBar.o : $(SRC_CORE)/StatusBar.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/StatusBar.cpp -o $(OUTDIR_AK_D)/StatusBar.o

$(OUTDIR_AK_D)/TabbedPages.o : $(SRC_CORE)/TabbedPages.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/TabbedPages.cpp -o $(OUTDIR_AK_D)/TabbedPages.o

$(OUTDIR_AK_D)/TableControl.o : $(SRC_CORE)/TableControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/TableControl.cpp -o $(OUTDIR_AK_D)/TableControl.o

$(OUTDIR_AK_D)/TableModelEvent.o : $(SRC_EVENT)/TableModelEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/TableModelEvent.cpp -o $(OUTDIR_AK_D)/TableModelEvent.o

$(OUTDIR_AK_D)/TabModelEvent.o : $(SRC_EVENT)/TabModelEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/TabModelEvent.cpp -o $(OUTDIR_AK_D)/TabModelEvent.o

$(OUTDIR_AK_D)/TextControl.o : $(SRC_CORE)/TextControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/TextControl.cpp -o $(OUTDIR_AK_D)/TextControl.o

$(OUTDIR_AK_D)/TextEvent.o : $(SRC_EVENT)/TextEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/TextEvent.cpp -o $(OUTDIR_AK_D)/TextEvent.o

$(OUTDIR_AK_D)/TimerComponent.o : $(SRC_CORE)/TimerComponent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/TimerComponent.cpp -o $(OUTDIR_AK_D)/TimerComponent.o

$(OUTDIR_AK_D)/TimerEvent.o : $(SRC_EVENT)/TimerEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/TimerEvent.cpp -o $(OUTDIR_AK_D)/TimerEvent.o

$(OUTDIR_AK_D)/ToggledButton.o : $(SRC_CORE)/ToggledButton.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ToggledButton.cpp -o $(OUTDIR_AK_D)/ToggledButton.o

$(OUTDIR_AK_D)/Toolbar.o : $(SRC_CORE)/Toolbar.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Toolbar.cpp -o $(OUTDIR_AK_D)/Toolbar.o

$(OUTDIR_AK_D)/ToolbarButton.o : $(SRC_CORE)/ToolbarButton.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ToolbarButton.cpp -o $(OUTDIR_AK_D)/ToolbarButton.o

$(OUTDIR_AK_D)/ToolbarDock.o : $(SRC_CORE)/ToolbarDock.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ToolbarDock.cpp -o $(OUTDIR_AK_D)/ToolbarDock.o

$(OUTDIR_AK_D)/ToolbarSeparator.o : $(SRC_CORE)/ToolbarSeparator.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/ToolbarSeparator.cpp -o $(OUTDIR_AK_D)/ToolbarSeparator.o

$(OUTDIR_AK_D)/ToolTipEvent.o : $(SRC_EVENT)/ToolTipEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ToolTipEvent.cpp -o $(OUTDIR_AK_D)/ToolTipEvent.o

$(OUTDIR_AK_D)/TreeControl.o : $(SRC_CORE)/TreeControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/TreeControl.cpp -o $(OUTDIR_AK_D)/TreeControl.o

$(OUTDIR_AK_D)/TreeListControl.o : $(SRC_CORE)/TreeListControl.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/TreeListControl.cpp -o $(OUTDIR_AK_D)/TreeListControl.o

$(OUTDIR_AK_D)/TreeModelEvent.o : $(SRC_EVENT)/TreeModelEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/TreeModelEvent.cpp -o $(OUTDIR_AK_D)/TreeModelEvent.o

$(OUTDIR_AK_D)/UIToolkit.o : $(SRC_UTILS)/UIToolkit.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_UTILS)/UIToolkit.cpp -o $(OUTDIR_AK_D)/UIToolkit.o

$(OUTDIR_AK_D)/UndoRedoEvent.o : $(SRC_EVENT)/UndoRedoEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/UndoRedoEvent.cpp -o $(OUTDIR_AK_D)/UndoRedoEvent.o

$(OUTDIR_AK_D)/UndoRedoStack.o : $(SRC_CORE)/UndoRedoStack.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/UndoRedoStack.cpp -o $(OUTDIR_AK_D)/UndoRedoStack.o

$(OUTDIR_AK_D)/ValidationEvent.o : $(SRC_EVENT)/ValidationEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/ValidationEvent.cpp -o $(OUTDIR_AK_D)/ValidationEvent.o

$(OUTDIR_AK_D)/VFFInputStream.o : $(SRC_IO)/VFFInputStream.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IO)/VFFInputStream.cpp -o $(OUTDIR_AK_D)/VFFInputStream.o

$(OUTDIR_AK_D)/VFFOutputStream.o : $(SRC_IO)/VFFOutputStream.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IO)/VFFOutputStream.cpp -o $(OUTDIR_AK_D)/VFFOutputStream.o

$(OUTDIR_AK_D)/WhatsThisHelpEvent.o : $(SRC_EVENT)/WhatsThisHelpEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/WhatsThisHelpEvent.cpp -o $(OUTDIR_AK_D)/WhatsThisHelpEvent.o

$(OUTDIR_AK_D)/Window.o : $(SRC_CORE)/Window.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_CORE)/Window.cpp -o $(OUTDIR_AK_D)/Window.o

$(OUTDIR_AK_D)/WindowEvent.o : $(SRC_EVENT)/WindowEvent.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_EVENT)/WindowEvent.cpp -o $(OUTDIR_AK_D)/WindowEvent.o

$(OUTDIR_AK_D)/X11UIToolkit.o : $(SRC_IMPLKIT)/X11UIToolkit.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IMPLKIT)/X11UIToolkit.cpp -o $(OUTDIR_AK_D)/X11UIToolkit.o

$(OUTDIR_AK_D)/X11Desktop.o : $(SRC_IMPLKIT)/X11Desktop.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IMPLKIT)/X11Desktop.cpp -o $(OUTDIR_AK_D)/X11Desktop.o

$(OUTDIR_AK_D)/X11Application.o : $(SRC_IMPLKIT)/X11Application.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IMPLKIT)/X11Application.cpp -o $(OUTDIR_AK_D)/X11Application.o

$(OUTDIR_AK_D)/AbstractX11Control.o : $(SRC_IMPLKIT)/AbstractX11Control.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IMPLKIT)/AbstractX11Control.cpp -o $(OUTDIR_AK_D)/AbstractX11Control.o

$(OUTDIR_AK_D)/X11Control.o : $(SRC_IMPLKIT)/X11Control.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IMPLKIT)/X11Control.cpp -o $(OUTDIR_AK_D)/X11Control.o

$(OUTDIR_AK_D)/X11Window.o : $(SRC_IMPLKIT)/X11Window.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IMPLKIT)/X11Window.cpp -o $(OUTDIR_AK_D)/X11Window.o

$(OUTDIR_AK_D)/X11ControlContext.o : $(SRC_IMPLKIT)/X11ControlContext.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IMPLKIT)/X11ControlContext.cpp -o $(OUTDIR_AK_D)/X11ControlContext.o

$(OUTDIR_AK_D)/X11Cursor.o : $(SRC_IMPLKIT)/X11Cursor.cpp $(APPKIT_HDRS)
	$(CXX) $(AK_CXX_FLAGS_D) $(SRC_IMPLKIT)/X11Cursor.cpp -o $(OUTDIR_AK_D)/X11Cursor.o


