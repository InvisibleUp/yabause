/*	Copyright 2012 Theo Berkau <cwx@cyberwarriorx.com>

	This file is part of Yabause.

	Yabause is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Yabause is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Yabause; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/
#include "UIDebugVDP1.h"
#include "CommonDialogs.h"

#include <QImageWriter>
#include <QGraphicsPixmapItem>
#include <QTimer>

UIDebugVDP1::UIDebugVDP1( QWidget* p )
	: QDialog( p )
{
	// setup dialog
	setupUi( this );

      QGraphicsScene *texscene=new QGraphicsScene(this);
      gvTexture->setScene(texscene);
      QGraphicsScene *grdscene=new QGraphicsScene(this);
      gvGouraud->setScene(grdscene);

      // retranslate widgets
	QtYabause::retranslateWidget( this );

      // init previews
      vdp1texture = NULL;
      vdp1texturew = vdp1textureh = 1;
      gouraudtexture = NULL;

      // Update list asyncronously
      timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), this, SLOT(UpdateList()));
      timer->setTimerType(Qt::PreciseTimer);
      timer->start(16);
      
}

UIDebugVDP1::~UIDebugVDP1()
{
   if (vdp1texture) free(vdp1texture);
}

void UIDebugVDP1::on_lwCommandList_itemSelectionChanged ()
{
      // Get selection
      int cursel = lwCommandList->currentRow();
      char tempstr[1024];

      // Print debugging text
      Vdp1DebugCommand(cursel, tempstr);
      pteCommandInfo->clear();
      pteCommandInfo->appendPlainText(QtYabause::translate(tempstr));
      pteCommandInfo->moveCursor(QTextCursor::Start);

      {
            // Get texture
            if (vdp1texture) free(vdp1texture);
            vdp1texture = Vdp1DebugTexture(cursel, &vdp1texturew, &vdp1textureh);
            pbSaveBitmap->setEnabled(vdp1texture ? true : false);

            // Redraw texture
            QGraphicsScene *scene = gvTexture->scene();
            QImage img((uchar *)vdp1texture, vdp1texturew, vdp1textureh, QImage::Format_ARGB32);
            QPixmap pixmap = QPixmap::fromImage(img.rgbSwapped());
            scene->clear();
            scene->addPixmap(pixmap);
            scene->setSceneRect(scene->itemsBoundingRect());
            gvTexture->fitInView(scene->sceneRect());
            gvTexture->invalidateScene();
      }

      {
            // Get gouraud preview
            if (gouraudtexture) free(gouraudtexture);
            gouraudtexture = Vdp1DebugGouraudOverlay(cursel, 128, 128);

            // Redraw gouraud preview
            QGraphicsScene *scene = gvGouraud->scene();
            QImage img((uchar *)gouraudtexture, 128, 128, QImage::Format_ARGB32);
            QPixmap pixmap = QPixmap::fromImage(img.rgbSwapped());
            scene->clear();
            scene->addPixmap(pixmap);
            scene->setSceneRect(scene->itemsBoundingRect());
            gvGouraud->fitInView(scene->sceneRect());
            gvGouraud->invalidateScene();
      }
}

void UIDebugVDP1::on_pbSaveBitmap_clicked ()
{
	QStringList filters;
	foreach ( QByteArray ba, QImageWriter::supportedImageFormats() )
		if ( !filters.contains( ba, Qt::CaseInsensitive ) )
			filters << QString( ba ).toLower();
	for ( int i = 0; i < filters.count(); i++ )
		filters[i] = QtYabause::translate( "%1 Images (*.%2)" ).arg( filters[i].toUpper() ).arg( filters[i] );
	
	// take screenshot of gl view
      QImage img((uchar *)vdp1texture, vdp1texturew, vdp1textureh, QImage::Format_ARGB32);
      img = img.rgbSwapped();
	
	// request a file to save to to user
	const QString s = CommonDialogs::getSaveFileName( QString(), QtYabause::translate( "Choose a location for your bitmap" ), filters.join( ";;" ) );
	
	// write image if ok
	if ( !s.isEmpty() )
		if ( !img.save( s ) )
			CommonDialogs::information( QtYabause::translate( "An error occured while writing file." ) );
}

// Populate command list
// TODO: Seperate all this into different comboboxes and stuff
void UIDebugVDP1::UpdateList()
{ 
      lwCommandList->clear();
      if (Vdp1Ram) {
            for (int i=0;;i++) {
                  // Get command type
                  char outstring[256];
                  Vdp1DebugGetCommandNumberName(i, outstring);
                  if (*outstring == '\0') break;

                  // Add to command list
                  lwCommandList->addItem(QtYabause::translate(outstring));
            }
      }
      pbSaveBitmap->setEnabled(vdp1texture ? true : false);
}