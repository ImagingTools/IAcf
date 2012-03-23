TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += Static
SUBDIRS += Component

Static.file = Static.pro
Component.file = Component.pro
Component.depends = Static
