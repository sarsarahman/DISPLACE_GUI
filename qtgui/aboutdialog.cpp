// --------------------------------------------------------------------------
// DISPLACE: DYNAMIC INDIVIDUAL VESSEL-BASED SPATIAL PLANNING
// AND EFFORT DISPLACEMENT
// Copyright (c) 2012-2018 Francois Bastardie <fba@aqua.dtu.dk>

//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// --------------------------------------------------------------------------

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <version.h>

#include <QFile>
#include <QTextStream>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QFile f(":/resources/about.txt");
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream s (&f);
        QString txt = s.readAll();

        txt.replace("$VERSION", VER_PRODUCTVERSION_STR);

        ui->aboutText->setTextFormat(Qt::RichText);
        ui->aboutText->setText(txt);
    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
