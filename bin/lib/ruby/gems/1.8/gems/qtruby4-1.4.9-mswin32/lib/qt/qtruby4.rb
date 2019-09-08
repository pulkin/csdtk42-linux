=begin
/***************************************************************************
                          qtruby.rb  -  description
                             -------------------
    begin                : Fri Jul 4 2003
    copyright            : (C) 2003-2005 by Richard Dale
    email                : Richard_Dale@tipitina.demon.co.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
=end

module Qt
	module DebugLevel
		Off, Minimal, High, Extensive = 0, 1, 2, 3
	end

	module QtDebugChannel 
		QTDB_NONE = 0x00
		QTDB_AMBIGUOUS = 0x01
		QTDB_METHOD_MISSING = 0x02
		QTDB_CALLS = 0x04
		QTDB_GC = 0x08
		QTDB_VIRTUAL = 0x10
		QTDB_VERBOSE = 0x20
		QTDB_ALL = QTDB_VERBOSE | QTDB_VIRTUAL | QTDB_GC | QTDB_CALLS | QTDB_METHOD_MISSING | QTDB_AMBIGUOUS
	end

	@@debug_level = DebugLevel::Off
	def Qt.debug_level=(level)
		@@debug_level = level
		Internal::setDebug Qt::QtDebugChannel::QTDB_ALL if level >= DebugLevel::Extensive
	end

	def Qt.debug_level
		@@debug_level
	end
		
	class Base
		def self.signals(*signal_list)
			meta = Qt::Meta[self.name] || Qt::MetaInfo.new(self)
			meta.add_signals(signal_list)
			meta.changed = true
		end
	
		def self.slots(*slot_list)
			meta = Qt::Meta[self.name] || Qt::MetaInfo.new(self)
			meta.add_slots(slot_list)
			meta.changed = true
		end

		def self.q_signal(signal)
			meta = Qt::Meta[self.name] || Qt::MetaInfo.new(self)
			meta.add_signals([signal])
			meta.changed = true
		end

		def self.q_slot(slot)
			meta = Qt::Meta[self.name] || Qt::MetaInfo.new(self)
			meta.add_slots([slot])
			meta.changed = true
		end

		def self.q_classinfo(key, value)
			meta = Qt::Meta[self.name] || Qt::MetaInfo.new(self)
			meta.add_classinfo(key, value)
			meta.changed = true
		end

		def **(a)
			return Qt::**(self, a)
		end
		def +(a)
			return Qt::+(self, a)
		end
		def ~(a)
			return Qt::~(self, a)
		end
		def -@()
			return Qt::-(self)
		end
		def -(a)
			return Qt::-(self, a)
		end
		def *(a)
			return Qt::*(self, a)
		end
		def /(a)
			return Qt::/(self, a)
		end
		def %(a)
			return Qt::%(self, a)
		end
		def >>(a)
			return Qt::>>(self, a)
		end
		def <<(a)
			return Qt::<<(self, a)
		end
		def &(a)
			return Qt::&(self, a)
		end
		def ^(a)
			return Qt::^(self, a)
		end
		def |(a)
			return Qt::|(self, a)
		end

#		Module has '<', '<=', '>' and '>=' operator instance methods, so pretend they
#		don't exist by calling method_missing() explicitely
		def <(a)
			begin
				Qt::method_missing(:<, self, a)
			rescue
				super(a)
			end
		end

		def <=(a)
			begin
				Qt::method_missing(:<=, self, a)
			rescue
				super(a)
			end
		end

		def >(a)
			begin
				Qt::method_missing(:>, self, a)
			rescue
				super(a)
			end
		end

		def >=(a)
			begin
				Qt::method_missing(:>=, self, a)
			rescue
				super(a)
			end
		end

#		Object has a '==' operator instance method, so pretend it
#		don't exist by calling method_missing() explicitely
		def ==(a)
			begin
				Qt::method_missing(:==, self, a)
			rescue
				super(a)
			end
		end


		def methods(regular=true)
			if !regular
				return singleton_methods
			end
	
			qt_methods(super, 0x0)
		end
	
		def protected_methods
			# From smoke.h, Smoke::mf_protected 0x80
			qt_methods(super, 0x80)
		end
	
		def public_methods
			methods
		end
	
		def singleton_methods
			# From smoke.h, Smoke::mf_static 0x01
			qt_methods(super, 0x01)
		end
	
		private
		def qt_methods(meths, flags)
			ids = []
			# These methods are all defined in Qt::Base, even if they aren't supported by a particular
			# subclass, so remove them to avoid confusion
			meths -= ["%", "&", "*", "**", "+", "-", "-@", "/", "<", "<<", "<=", ">", ">=", ">>", "|", "~", "^"]
			classid = Qt::Internal::idInstance(self)
			Qt::Internal::getAllParents(classid, ids)
			ids << classid
			ids.each { |c| Qt::Internal::findAllMethodNames(meths, c, flags) }
			return meths.uniq
		end
	end # Qt::Base

	class AbstractSocket < Qt::Base
		def abort(*args)
			method_missing(:abort, *args)
		end
	end

	class AbstractTextDocumentLayout < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class AccessibleEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class ActionEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class Action < Qt::Base 
		def setShortcut(arg)
			if arg.kind_of?(String)
				return super(Qt::KeySequence.new(arg))
			else
				return super(arg)
			end
		end

		def shortcut=(arg)
			setShortcut(arg)
		end
	end

	class Application < Qt::Base
		# Delete the underlying C++ instance after exec returns
		# Otherwise, rb_gc_call_finalizer_at_exit() can delete
		# stuff that Qt::Application still needs for its cleanup.
		def exec
			method_missing(:exec)
			self.dispose
			Qt::Internal.application_terminated = true
		end

		def type(*args)
			method_missing(:type, *args)
		end
	end

	class Buffer < Qt::Base
		def open(*args)
			method_missing(:open, *args)
		end
	end

	class ButtonGroup < Qt::Base
		def id(*args)
			method_missing(:id, *args)
		end
	end
	
	class ByteArray < Qt::Base
		def to_s
			return constData()
		end

		def to_i
			return toInt()
		end

		def to_f
			return toDouble()
		end

		def chop(*args)
			method_missing(:chop, *args)
		end

		def split(*args)
			method_missing(:split, *args)
		end
	end
	
	class CheckBox < Qt::Base 
		def setShortcut(arg)
			if arg.kind_of?(String)
				return super(Qt::KeySequence.new(arg))
			else
				return super(arg)
			end
		end

		def shortcut=(arg)
			setShortcut(arg)
		end
	end

	class ChildEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class CloseEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end
	
	class Color < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " %s>" % name)
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, " %s>" % name)
		end

		def name(*args)
			method_missing(:name, *args)
		end
	end
	
	class Connection < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " memberName=%s, memberType=%s, object=%s>" %
				[memberName.inspect, memberType == 1 ? "SLOT" : "SIGNAL", object.inspect] )
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n memberName=%s,\n memberType=%s,\n object=%s>" %
				[memberName.inspect, memberType == 1 ? "SLOT" : "SIGNAL", object.inspect] )
		end
	end

	class ContextMenuEvent < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class CoreApplication < Qt::Base
		def exec
			method_missing(:exec)
			self.dispose
			Qt::Internal.application_terminated = true
		end

		def exit(*args)
			method_missing(:exit, *args)
		end
	end
	
	class Cursor < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " shape=%d>" % shape)
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, " shape=%d>" % shape)
		end
	end

	class CustomEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end
	
	class Date < Qt::Base

		def initialize(*args)
			if args.size == 1 && args[0].class.name == "Date"
				return super(args[0].year, args[0].month, args[0].day)
			else
				return super(*args)
			end
		end

		def inspect
			str = super
			str.sub(/>$/, " %s>" % toString)
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, " %s>" % toString)
		end
	end
	
	class DateTime < Qt::Base
		def initialize(*args)
			if args.size == 1 && args[0].class.name == "DateTime"
				return super(	Qt::Date.new(args[0].year, args[0].month, args[0].day), 
								Qt::Time.new(args[0].hour, args[0].min, args[0].sec) )
			else
				return super(*args)
			end
		end

		def inspect
			str = super
			str.sub(/>$/, " %s>" % toString)
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, " %s>" % toString)
		end
	end

	class DBusConnection < Qt::Base
		def send(*args)
			method_missing(:send, *args)
		end
	end

	class DBusConnectionInterface < Qt::Base
		def serviceOwner(name)
    		return Qt::DBusReply.new(internalConstCall(Qt::DBus::AutoDetect, "GetNameOwner", [Qt::Variant.new(name)]))
		end

		def registeredServiceNames
			return Qt::DBusReply.new(internalConstCall(Qt::DBus::AutoDetect, "ListNames"))
		end

		def isServiceRegistered(serviceName)
    		return Qt::DBusReply.new(internalConstCall(Qt::DBus::AutoDetect, "NameHasOwner", [Qt::Variant.new(serviceName)]))
		end

		def serviceRegistered?(serviceName)
    		return isServiceRegistered(serviceName)
		end

		def servicePid(serviceName)
    		return Qt::DBusReply.new(internalConstCall(Qt::DBus::AutoDetect, "GetConnectionUnixProcessID", [Qt::Variant.new(serviceName)]))
		end

		def serviceUid(serviceName)
    		return Qt::DBusReply.new(internalConstCall(Qt::DBus::AutoDetect, "GetConnectionUnixUser", [Qt::Variant.new(serviceName)]))
		end

		def startService(name)
    		return call("StartServiceByName", Qt::Variant.new(name), Qt::Variant.new(0)).value
		end
	end

	class DBusError < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class DBusInterface < Qt::Base 

		def call(method_name, *args)
			if args.length == 0
				return super(method_name)
			else
				# If the method is Qt::DBusInterface.call(), create an Array 
				# 'dbusArgs' of Qt::Variants from '*args'
				qdbusArgs = args.collect {|arg| qVariantFromValue(arg)}
				return super(method_name, *qdbusArgs)
			end
		end

		def method_missing(id, *args)
			begin
				# First look for a method in the Smoke runtime
				# If not found, then throw an exception and try dbus.
				super(id, *args)
			rescue
				if args.length == 0
					return call(id.to_s).value
				else
					return call(id.to_s, *args).value
				end
			end
		end
	end

	class DBusMessage < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end

		def value
			if type() == Qt::DBusMessage::ReplyMessage
				reply = arguments()
				return reply.length > 0 ? reply[0].value : nil
			else
				return nil
			end
		end

		def <<(a)
			if a.kind_of?(Qt::Variant)
				return super(a)
			else
				return super(qVariantFromValue(a))
			end
		end
	end

	class DBusReply
		def initialize(reply)
			@error = Qt::DBusError.new(reply)

			if @error.valid?
				@data = Qt::Variant.new
				return
			end

			if reply.arguments.length >= 1
				@data = reply.arguments[0]
				return
			end
			
			# error
			@error = Qt::DBusError.new(	Qt::DBusError::InvalidSignature, 
										"Unexpected reply signature" )
			@data = Qt::Variant.new      # clear it
		end

		def isValid
			return !@error.isValid
		end

		def valid?
			return !@error.isValid
		end

		def value
			return @data.value
		end

		def error
			return @error
		end
	end

	class Dialog < Qt::Base
		def exec(*args)
			method_missing(:exec, *args)
		end
	end

	class DomAttr < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end
	end

	class DomDocumentType < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end

		def type(*args)
			method_missing(:type, *args)
		end
	end

	class DragEnterEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class DragLeaveEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class DropEvent < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class Event < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class EventLoop < Qt::Base
		def exec(*args)
			method_missing(:exec, *args)
		end

		def exit(*args)
			method_missing(:exit, *args)
		end
	end

	class File < Qt::Base
		def open(*args)
			method_missing(:open, *args)
		end
	end

	class FileOpenEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class FileIconProvider < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class FocusEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end
	
	class Font < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " family=%s, pointSize=%d, weight=%d, italic=%s, bold=%s, underline=%s, strikeOut=%s>" % 
			[family.inspect, pointSize, weight, italic, bold, underline, strikeOut])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n family=%s,\n pointSize=%d,\n weight=%d,\n italic=%s,\n bold=%s,\n underline=%s,\n strikeOut=%s>" % 
			[family.inspect, pointSize, weight, italic, bold, underline, strikeOut])
		end
	end

	class Ftp < Qt::Base
		def abort(*args)
			method_missing(:abort, *args)
		end
	end

	class GLContext < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class GLPixelBuffer < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class GLWidget < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class GenericArgument < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end
	end

	class Gradient < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsEllipseItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsItemGroup < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsLineItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsPathItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsPixmapItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsPolygonItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsRectItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsSceneMouseEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsSceneContextMenuEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsSceneHoverEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsSceneHelpEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsSceneWheelEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsSimpleTextItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class GraphicsTextItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class HelpEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class HideEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class HoverEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class Http < Qt::Base
		def abort(*args)
			method_missing(:abort, *args)
		end
	end

	class HttpRequestHeader < Qt::Base
		def method(*args)
			if args.length == 1
				super(*args)
			else
				method_missing(:method, *args)
			end
		end
	end

	class IconDragEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class InputEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class InputMethodEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class IODevice < Qt::Base
		def open(*args)
			method_missing(:open, *args)
		end
	end

	class Image < Qt::Base
		def fromImage(image)
			send("operator=".to_sym, image)
		end

		def format(*args)
			method_missing(:format, *args)
		end

		def load(*args)
			method_missing(:load, *args)
		end
	end

	class ImageIOHandler < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end

		def name(*args)
			method_missing(:name, *args)
		end
	end

	class ImageReader < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class ImageWriter < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class ItemSelection < Qt::Base
		def select(*args)
			method_missing(:select, *args)
		end

		def split(*args)
			method_missing(:split, *args)
		end
	end

	class ItemSelectionModel < Qt::Base
		def select(*args)
			method_missing(:select, *args)
		end
	end
	
	class KeySequence < Qt::Base
		def initialize(*args)
			if args.length == 1 && args[0].kind_of?(Qt::Enum) && args[0].type == "Qt::Key"
				return super(args[0].to_i)
			end
			return super(*args)
		end
	end

	class LCDNumber < Qt::Base
		def display(item)
			method_missing(:display, item)
		end
	end

	class Library < Qt::Base
		def load(*args)
			method_missing(:load, *args)
		end
	end

	class ListWidgetItem < Qt::Base
		def clone(*args)
			method_missing(:clone, *args)
		end

		def type(*args)
			method_missing(:type, *args)
		end

		def inspect
			str = super
			str.sub(/>$/, " text=%s>" % text)
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, " text=%s>" % text)
		end
	end

	class Locale < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end

		def system(*args)
			method_missing(:system, *args)
		end
	end

	class Menu < Qt::Base
		def exec(*args)
			method_missing(:exec, *args)
		end
	end

	class MetaClassInfo < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end
	end

	class MetaEnum < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end

		def keyValues()
			res = []
			for i in 0...keyCount()
				if flag?
					res.push "%s=0x%x" % [key(i), value(i)]
				else
					res.push "%s=%d" % [key(i), value(i)]
				end
			end
			return res
		end

		def inspect
			str = super
			str.sub(/>$/, " scope=%s, name=%s, keyValues=Array (%d element(s))>" % [scope, name, keyValues.length])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, " scope=%s, name=%s, keyValues=Array (%d element(s))>" % [scope, name, keyValues.length])
		end
	end

	class MetaMethod < Qt::Base
		# Oops, name clash with the Signal module so hard code
		# this value rather than get it from the Smoke runtime
		Signal = 1
	end

	class MetaObject < Qt::Base
		def method(*args)
            if args.length == 1 && args[0].kind_of?(Symbol)
				super(*args)
			else
				method_missing(:method, *args)
			end
		end

		# Add three methods, 'propertyNames()', 'slotNames()' and 'signalNames()'
		# from Qt3, as they are very useful when debugging

		def propertyNames(inherits = false)
			res = []
			if inherits
				for p in 0...propertyCount() 
					res.push property(p).name
				end
			else
				for p in propertyOffset()...propertyCount()
					res.push property(p).name
				end
			end
			return res
		end

		def slotNames(inherits = false)
			res = []
			if inherits
				for m in 0...methodCount()
					if method(m).methodType == Qt::MetaMethod::Slot 
						res.push "%s %s" % [method(m).typeName == "" ? "void" : method(m).typeName, 
											method(m).signature]
					end
				end
			else
				for m in methodOffset()...methodCount()
					if method(m).methodType == Qt::MetaMethod::Slot 
						res.push "%s %s" % [method(m).typeName == "" ? "void" : method(m).typeName, 
											method(m).signature]
					end
				end
			end
			return res
		end

		def signalNames(inherits = false)
			res = []
			if inherits
				for m in 0...methodCount()
					if method(m).methodType == Qt::MetaMethod::Signal 
						res.push "%s %s" % [method(m).typeName == "" ? "void" : method(m).typeName, 
											method(m).signature]
					end
				end
			else
				for m in methodOffset()...methodCount()
					if method(m).methodType == Qt::MetaMethod::Signal 
						res.push "%s %s" % [method(m).typeName == "" ? "void" : method(m).typeName, 
											method(m).signature]
					end
				end
			end
			return res
		end

		def enumerators(inherits = false)
			res = []
			if inherits
				for e in 0...enumeratorCount()
					res.push enumerator(e)
				end
			else
				for e in enumeratorOffset()...enumeratorCount()
					res.push enumerator(e)
				end
			end
			return res
		end

		def inspect
			str = super
			str.sub!(/>$/, "")
			str << " className=%s," % className
			str << " propertyNames=Array (%d element(s))," % propertyNames.length unless propertyNames.length == 0
			str << " signalNames=Array (%d element(s))," % signalNames.length unless signalNames.length == 0
			str << " slotNames=Array (%d element(s))," % slotNames.length unless slotNames.length == 0
			str << " enumerators=Array (%d element(s))," % enumerators.length unless enumerators.length == 0
			str << " superClass=%s," % superClass.inspect unless superClass == nil
			str.chop!
			str << ">"
		end
		
		def pretty_print(pp)
			str = to_s
			str.sub!(/>$/, "")
			str << "\n className=%s," % className
			str << "\n propertyNames=Array (%d element(s))," % propertyNames.length unless propertyNames.length == 0
			str << "\n signalNames=Array (%d element(s))," % signalNames.length unless signalNames.length == 0
			str << "\n slotNames=Array (%d element(s))," % slotNames.length unless slotNames.length == 0
			str << "\n enumerators=Array (%d element(s))," % enumerators.length unless enumerators.length == 0
			str << "\n superClass=%s," % superClass.inspect unless superClass == nil
			str << "\n methodCount=%d," % methodCount
			str << "\n methodOffset=%d," % methodOffset
			str << "\n propertyCount=%d," % propertyCount
			str << "\n propertyOffset=%d," % propertyOffset
			str << "\n enumeratorCount=%d," % enumeratorCount
			str << "\n enumeratorOffset=%d," % enumeratorOffset
			str.chop!
			str << ">"
			pp.text str
		end
	end

	class MetaProperty < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end

		def type(*args)
			method_missing(:type, *args)
		end
	end

	class MetaType < Qt::Base
		def load(*args)
			method_missing(:load, *args)
		end

		def type(*args)
			method_missing(:type, *args)
		end
	end

	class MouseEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class MoveEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class Movie < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class NetworkProxy < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class Object < Qt::Base
	end

	class PageSetupDialog < Qt::Base
		def exec(*args)
			method_missing(:exec, *args)
		end
	end

	class PaintEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class Picture < Qt::Base
		def load(*args)
			method_missing(:load, *args)
		end
	end

	class PictureIO < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class Pixmap < Qt::Base
		def load(*args)
			method_missing(:load, *args)
		end
	end

	class PluginLoader < Qt::Base
		def load(*args)
			method_missing(:load, *args)
		end
	end
	
	class Point < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " x=%d, y=%d>" % [x, y])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n x=%d,\n y=%d>" % [x, y])
		end
	end
	
	class PointF < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " x=%f, y=%f>" % [x, y])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n x=%f,\n y=%f>" % [x, y])
		end
	end

	class PrintDialog < Qt::Base
		def exec(*args)
			method_missing(:exec, *args)
		end
	end

	class Printer < Qt::Base
		def abort(*args)
			method_missing(:abort, *args)
		end
	end
	
	class PushButton < Qt::Base 
		def setShortcut(arg)
			if arg.kind_of?(String)
				return super(Qt::KeySequence.new(arg))
			else
				return super(arg)
			end
		end

		def shortcut=(arg)
			setShortcut(arg)
		end
	end

	class Line < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " x1=%d, y1=%d, x2=%d, y2=%d>" % [x1, y1, x2, y2])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n x1=%d,\n y1=%d,\n x2=%d,\n y2=%d>" % [x1, y1, x2, y2])
		end
	end
	
	class LineF < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " x1=%f, y1=%f, x2=%f, y2=%f>" % [x1, y1, x2, y2])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n x1=%f,\n y1=%f,\n x2=%f,\n y2=%f>" % [x1, y1, x2, y2])
		end
	end
	
	class MetaType < Qt::Base
		def self.type(*args)
			method_missing(:type, *args)
		end
	end

	class ModelIndex < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " valid?=%s, row=%s, column=%s>" % [valid?, row, column])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n valid?=%s,\n row=%s,\n column=%s>" % [valid?, row, column])
		end
	end
	
	class RadioButton < Qt::Base 
		def setShortcut(arg)
			if arg.kind_of?(String)
				return super(Qt::KeySequence.new(arg))
			else
				return super(arg)
			end
		end

		def shortcut=(arg)
			setShortcut(arg)
		end
	end

	class Rect < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " left=%d, right=%d, top=%d, bottom=%d>" % [left, right, top, bottom])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n left=%d,\n right=%d,\n top=%d,\n bottom=%d>" % [left, right, top, bottom])
		end
	end
	
	class RectF < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " left=%f, right=%f, top=%f, bottom=%f>" % [left, right, top, bottom])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n left=%f,\n right=%f,\n top=%f,\n bottom=%f>" % [left, right, top, bottom])
		end
	end

	class ResizeEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class Shortcut < Qt::Base
		def id(*args)
			method_missing(:id, *args)
		end
	end

	class ShortcutEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class ShowEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end
	
	class Size < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " width=%d, height=%d>" % [width, height])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n width=%d,\n height=%d>" % [width, height])
		end
	end
	
	class SizeF < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " width=%f, height=%f>" % [width, height])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n width=%f,\n height=%f>" % [width, height])
		end
	end
	
	class SizePolicy < Qt::Base
		def inspect
			str = super
			str.sub(/>$/, " horData=%d, verData=%d>" % [horData, verData])
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, "\n horData=%d,\n verData=%d>" % [horData, verData])
		end
	end

	class SocketNotifier < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class SqlDatabase < Qt::Base
		def exec(*args)
			method_missing(:exec, *args)
		end

		def open(*args)
			method_missing(:open, *args)
		end
	end

	class SqlError < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class SqlField < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end

		def type(*args)
			method_missing(:type, *args)
		end
	end

	class SqlIndex < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end
	end

	class SqlQuery < Qt::Base
		def exec(*args)
			method_missing(:exec, *args)
		end
	end

	class SqlResult < Qt::Base
		def exec(*args)
			method_missing(:exec, *args)
		end
	end

	class SqlTableModel < Qt::Base
		def select(*k)
			method_missing(:select, *k)
		end
	end

	class StandardItem < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class StandardItemModel < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class StatusTipEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class StyleHintReturn < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class StyleOption < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class SyntaxHighlighter < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class TableWidgetItem < Qt::Base
		def clone(*args)
			method_missing(:clone, *args)
		end

		def type(*args)
			method_missing(:type, *args)
		end

		def inspect
			str = super
			str.sub(/>$/, " text=%s>" % text)
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, " text=%s>" % text)
		end
	end

	class TemporaryFile < Qt::Base
		def open(*args)
			method_missing(:open, *args)
		end
	end
	
	class TextCursor < Qt::Base
		def select(*k)
			method_missing(:select, *k)
		end
	end

	class TextDocument < Qt::Base
		def clone(*args)
			method_missing(:clone, *args)
		end

		def print(*args)
			method_missing(:print, *args)
		end
	end

	class TextFormat < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class TextImageFormat < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end
	end

	class TextInlineObject < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class TextLength < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class TextList < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class TextObject < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class TextTable < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end

	class TextTableCell < Qt::Base
		def format(*args)
			method_missing(:format, *args)
		end
	end
	
	class Time < Qt::Base
		def initialize(*args)
			if args.size == 1 && args[0].class.name == "Time"
				return super(args[0].hour, args[0].min, args[0].sec)
			else
				return super(*args)
			end
		end

		def inspect
			str = super
			str.sub(/>$/, " %s>" % toString)
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, " %s>" % toString)
		end
	end

	class TimerEvent < Qt::Base 
		def type(*args)
			method_missing(:type, *args)
		end
	end
	
	class ToolButton < Qt::Base 
		def setShortcut(arg)
			if arg.kind_of?(String)
				return super(Qt::KeySequence.new(arg))
			else
				return super(arg)
			end
		end

		def shortcut=(arg)
			setShortcut(arg)
		end
	end

	class Translator < Qt::Base
		def load(*args)
			method_missing(:load, *args)
		end
	end

	class TreeWidget < Qt::Base
		include Enumerable

		def each
			it = Qt::TreeWidgetItemIterator.new(self)
			while it.current
				yield it.current
				it += 1
			end
		end
	end

	class TreeWidgetItem < Qt::Base
		include Enumerable

		def initialize(*args)
			# There is not way to distinguish between the copy constructor
			# QTreeWidgetItem (const QTreeWidgetItem & other) 
			# and
			# QTreeWidgetItem (QTreeWidgetItem * parent, const QStringList & strings, int type = Type)
			# when the latter has a single argument. So force the second variant to be called
			if args.length == 1 && args[0].kind_of?(Qt::TreeWidgetItem)
				super(args[0], Qt::TreeWidgetItem::Type)
			else
				super(*args)
			end
		end

		def inspect
			str = super
			str.sub!(/>$/, "")
			str << " parent=%s," % parent unless parent.nil?
			for i in 0..(columnCount - 1)
				str << " text%d=%s," % [i, self.text(i)]
			end
			str.sub!(/,?$/, ">")
		end
		
		def pretty_print(pp)
			str = to_s
			str.sub!(/>$/, "")
			str << " parent=%s," % parent unless parent.nil?
			for i in 0..(columnCount - 1)
				str << " text%d=%s," % [i, self.text(i)]
			end
			str.sub!(/,?$/, ">")
			pp.text str
		end

		def clone(*args)
			method_missing(:clone, *args)
		end

		def type(*args)
			method_missing(:type, *args)
		end

		def each
			it = Qt::TreeWidgetItemIterator.new(self)
			while it.current
				yield it.current
				it += 1
			end
		end
	end

	class TreeWidgetItemIterator < Qt::Base
		def current
			return send("operator*".to_sym)
		end
	end

	class UrlInfo < Qt::Base
		def name(*args)
			method_missing(:name, *args)
		end
	end
	
	class Variant < Qt::Base
		String = 10
		Date = 14
		Time = 15
		DateTime = 16

		def initialize(*args)
			if args.size == 1 && args[0].nil?
				return super()
			elsif args.size == 1 && args[0].class.name == "Date"
				return super(Qt::Date.new(args[0]))
			elsif args.size == 1 && args[0].class.name == "DateTime"
				return super(Qt::DateTime.new(	Qt::Date.new(args[0].year, args[0].month, args[0].day), 
												Qt::Time.new(args[0].hour, args[0].min, args[0].sec) ) )
			elsif args.size == 1 && args[0].class.name == "Time"
				return super(Qt::Time.new(args[0]))
			else
				return super(*args)
			end
		end

		def to_a
			return toStringList()
		end

		def to_f
			return toDouble()
		end

		def to_i
			return toInt()
		end

		def to_int
			return toInt()
		end

		def value
			case type()
			when Qt::Variant::Bitmap
			when Qt::Variant::Bool
				return toBool
			when Qt::Variant::Brush
				return qVariantValue(Qt::Brush, self)
			when Qt::Variant::ByteArray
				return toByteArray
			when Qt::Variant::Char
				return qVariantValue(Qt::Char, self)
			when Qt::Variant::Color
				return qVariantValue(Qt::Color, self)
			when Qt::Variant::Cursor
				return qVariantValue(Qt::Cursor, self)
			when Qt::Variant::Date
				return toDate
			when Qt::Variant::DateTime
				return toDateTime
			when Qt::Variant::Double
				return toDouble
			when Qt::Variant::Font
				return qVariantValue(Qt::Font, self)
			when Qt::Variant::Icon
				return qVariantValue(Qt::Icon, self)
			when Qt::Variant::Image
				return qVariantValue(Qt::Image, self)
			when Qt::Variant::Int
				return toInt
			when Qt::Variant::KeySequence
				return qVariantValue(Qt::KeySequence, self)
			when Qt::Variant::Line
				return toLine
			when Qt::Variant::LineF
				return toLineF
			when Qt::Variant::List
				return toList
			when Qt::Variant::Locale
				return qVariantValue(Qt::Locale, self)
			when Qt::Variant::LongLong
				return toLongLong
			when Qt::Variant::Map
				return toMap
			when Qt::Variant::Palette
				return qVariantValue(Qt::Palette, self)
			when Qt::Variant::Pen
				return qVariantValue(Qt::Pen, self)
			when Qt::Variant::Pixmap
				return qVariantValue(Qt::Pixmap, self)
			when Qt::Variant::Point
				return toPoint
			when Qt::Variant::PointF
				return toPointF
			when Qt::Variant::Polygon
				return qVariantValue(Qt::Polygon, self)
			when Qt::Variant::Rect
				return toRect
			when Qt::Variant::RectF
				return toRectF
			when Qt::Variant::RegExp
				return toRegExp
			when Qt::Variant::Region
				return qVariantValue(Qt::Region, self)
			when Qt::Variant::Size
				return toSize
			when Qt::Variant::SizeF
				return toSizeF
			when Qt::Variant::SizePolicy
				return toSizePolicy
			when Qt::Variant::String
				return toString
			when Qt::Variant::StringList
				return toStringList
			when Qt::Variant::TextFormat
				return qVariantValue(Qt::TextFormat, self)
			when Qt::Variant::TextLength
				return qVariantValue(Qt::TextLength, self)
			when Qt::Variant::Time
				return toTime
			when Qt::Variant::UInt
				return toUInt
			when Qt::Variant::ULongLong
				return toULongLong
			when Qt::Variant::Url
				return toUrl
			end

            case typeName()
            when "QDBusArgument"
				return qVariantValue(Qt::DBusArgument, self)
            when "QDBusVariant"
				return qVariantValue(Qt::Variant, self)
            end
		end

		def inspect
			str = super
			str.sub(/>$/, " typeName=%s>" % typeName)
		end
		
		def pretty_print(pp)
			str = to_s
			pp.text str.sub(/>$/, " typeName=%s>" % typeName)
		end

		def load(*args)
			method_missing(:load, *args)
		end

		def type(*args)
			method_missing(:type, *args)
		end
	end

	class DBusVariant < Variant
		def initialize(value)
			if value.kind_of? Qt::Variant
				super(value)
			else 
				super(Qt::Variant.new(value))
			end
		end

		def setVariant(variant)
		end

		def variant=(variant)
			setVariant(variant)
		end

		def variant()
			return self
		end
	end

	class WhatsThisClickedEvent < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class Widget < Qt::Base
		def raise(*args)
			method_missing(:raise, *args)
		end
	end

	class WindowStateChangeEvent < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end

	class XmlAttributes < Qt::Base
		def type(*args)
			method_missing(:type, *args)
		end
	end
	
	# Provides a mutable numeric class for passing to methods with
	# C++ 'int*' or 'int&' arg types
	class Integer
		attr_accessor :value
		def initialize(n=0) @value = n end
		
		def +(n) 
			return Integer.new(@value + n.to_i) 
		end
		def -(n) 
			return Integer.new(@value - n.to_i)
		end
		def *(n) 
			return Integer.new(@value * n.to_i)
		end
		def /(n) 
			return Integer.new(@value / n.to_i)
		end
		def %(n) 
			return Integer.new(@value % n.to_i)
		end
		def **(n) 
			return Integer.new(@value ** n.to_i)
		end
		
		def |(n) 
			return Integer.new(@value | n.to_i)
		end
		def &(n) 
			return Integer.new(@value & n.to_i)
		end
		def ^(n) 
			return Integer.new(@value ^ n.to_i)
		end
		def <<(n) 
			return Integer.new(@value << n.to_i)
		end
		def >>(n) 
			return Integer.new(@value >> n.to_i)
		end
		def >(n) 
			return @value > n.to_i
		end
		def >=(n) 
			return @value >= n.to_i
		end
		def <(n) 
			return @value < n.to_i
		end
		def <=(n) 
			return @value <= n.to_i
		end
		
		def <=>(n)
			if @value < n.to_i
				return -1
			elsif @value > n.to_i
				return 1
			else
				return 0
			end
		end
		
		def to_f() return @value.to_f end
		def to_i() return @value.to_i end
		def to_s() return @value.to_s end
		
		def coerce(n)
			[n, @value]
		end
	end
	
	# If a C++ enum was converted to an ordinary ruby Integer, the
	# name of the type is lost. The enum type name is needed for overloaded
	# method resolution when two methods differ only by an enum type.
	class Enum
		attr_accessor :type, :value
		def initialize(n, type)
			super() 
			@value = n 
			@type = type
		end
		
		def +(n) 
			return @value + n.to_i
		end
		def -(n) 
			return @value - n.to_i
		end
		def *(n) 
			return @value * n.to_i
		end
		def /(n) 
			return @value / n.to_i
		end
		def %(n) 
			return @value % n.to_i
		end
		def **(n) 
			return @value ** n.to_i
		end
		
		def |(n) 
			return Enum.new(@value | n.to_i, @type)
		end
		def &(n) 
			return Enum.new(@value & n.to_i, @type)
		end
		def ^(n) 
			return Enum.new(@value ^ n.to_i, @type)
		end
		def ~() 
			return ~ @value
		end
		def <(n) 
			return @value < n.to_i
		end
		def <=(n) 
			return @value <= n.to_i
		end
		def >(n) 
			return @value > n.to_i
		end
		def >=(n) 
			return @value >= n.to_i
		end
		def <<(n) 
			return Enum.new(@value << n.to_i, @type)
		end
		def >>(n) 
			return Enum.new(@value >> n.to_i, @type)
		end
		
		def ==(n) return @value == n.to_i end
		def to_i() return @value end

		def to_f() return @value.to_f end
		def to_s() return @value.to_s end
		
		def coerce(n)
			[n, @value]
		end
		
		def inspect
			to_s
		end

		def pretty_print(pp)
			pp.text "#<%s:0x%8.8x @type=%s, @value=%d>" % [self.class.name, object_id, type, value]
		end
	end
	
	# Provides a mutable boolean class for passing to methods with
	# C++ 'bool*' or 'bool&' arg types
	class Boolean
		attr_accessor :value
		def initialize(b=false) @value = b end
		def nil? 
			return !@value 
		end
	end

	class SignalBlockInvocation < Qt::Object
		def initialize(parent, block, signature)
			super(parent)
			if metaObject.indexOfSlot(signature) == -1
				self.class.slots signature
			end
			@block = block
		end

		def invoke(*args)
			@block.call(*args)
		end
	end

	class BlockInvocation < Qt::Object
		def initialize(target, block, signature)
			super(target)
			if metaObject.indexOfSlot(signature) == -1
				self.class.slots signature
			end
			@target = target
			@block = block
		end

		def invoke(*args)
			@target.instance_exec(*args, &@block)
		end
	end
	
	module Internal
		@@classes   = {}
		@@cpp_names = {}
		@@idclass   = []

		def Internal.normalize_classname(classname)
			if classname =~ /^Qext/
				now = classname.sub(/^Qext(?=[A-Z])/,'Qext::')
			elsif classname =~ /^Qwt/
				now = classname.sub(/^Qwt(?=[A-Z])/,'Qwt::')
			elsif classname =~ /^Q3/
				now = classname.sub(/^Q3(?=[A-Z])/,'Qt3::')
			elsif classname =~ /^Q/
				now = classname.sub(/^Q(?=[A-Z])/,'Qt::')
			elsif classname =~ /^(KConfigSkeleton|KWin)::/
				now = classname.sub(/^K?(?=[A-Z])/,'KDE::')
			elsif classname !~ /::/
				now = classname.sub(/^K?(?=[A-Z])/,'KDE::')
			else
				now = classname
			end
#			puts "normalize_classname = was::#{classname}, now::#{now}"
			now
		end

		def Internal.init_class(c)
			classname = Qt::Internal::normalize_classname(c)
			classId = Qt::Internal.idClass(c)
			insert_pclassid(classname, classId)
			@@idclass[classId] = classname
			@@cpp_names[classname] = c
			klass = isQObject(classId) ? create_qobject_class(classname) \
                                                   : create_qt_class(classname)
			@@classes[classname] = klass unless klass.nil?
		end

		def Internal.debug_level
			Qt.debug_level
		end

		def Internal.checkarg(argtype, typename)
			puts "      #{typename} (#{argtype})" if debug_level >= DebugLevel::High
			if argtype == 'i'
				if typename =~ /^int&?$|^signed int&?$|^signed$|^qint32&?$|^quint32&?$/
					return 2
				elsif typename =~ /^(?:short|ushort|unsigned short int|uchar|uint|long|ulong|unsigned long int|unsigned|float|double|WId|Q_PID|^quint16&?$|^qint16&?$)$/
					return 1
				elsif typename =~ /^(quint|qint|qulong|qlong|qreal)/
					return 1
				else 
					t = typename.sub(/^const\s+/, '')
					t.sub!(/[&*]$/, '')
					if isEnum(t)
						return 0
					end
				end
			elsif argtype == 'n'
				if typename =~ /^double$|^qreal$/
					return 2
				elsif typename =~ /^float$/
					return 1
				elsif typename =~ /^int&?$/
					return 0
				elsif typename =~ /^(?:short|ushort|uint|long|ulong|signed|unsigned|float|double)$/
					return 0
				else 
					t = typename.sub(/^const\s+/, '')
					t.sub!(/[&*]$/, '')
					if isEnum(t)
						return 0
					end
				end
			elsif argtype == 'B'
				if typename =~ /^(?:bool)[*&]?$/
					return 0
				end
			elsif argtype == 's'
				if typename =~ /^(const )?((QChar)[*&]?)$/
					return 1
				elsif typename =~ /^(?:u?char\*|const u?char\*|(?:const )?(Q(C?)String)[*&]?)$/
					qstring = !$1.nil?
					c = ("C" == $2)
					return c ? 2 : (qstring ? 3 : 0)
				end
			elsif argtype == 'a'
				# FIXME: shouldn't be hardcoded. Installed handlers should tell what ruby type they expect.
				if typename =~ /^(?:
						const\ QCOORD\*|
						(?:const\ )?
						(?:
						    QStringList[\*&]?|
						    QValueList<int>[\*&]?|
						    QRgb\*|
						    char\*\*
						)
					        )$/x
					return 0
				end
			elsif argtype == 'u'
				# Give nil matched against string types a higher score than anything else
				if typename =~ /^(?:u?char\*|const u?char\*|(?:const )?((Q(C?)String))[*&]?)$/
					return 1
				# Numerics will give a runtime conversion error, so they fail the match
				elsif typename =~ /^(?:short|ushort|uint|long|ulong|signed|unsigned|int)$/
					return -99
				else
					return 0
				end
			elsif argtype == 'U'
				if typename =~ /QStringList/
					return 1
				else
					return 0
				end
			else
				t = typename.sub(/^const\s+/, '')
				t.sub!(/[&*]$/, '')
				if argtype == t
					return 1
				elsif classIsa(argtype, t)
					return 0
				elsif isEnum(argtype) and 
						(t =~ /int|qint32|uint|quint32|long|ulong/ or isEnum(t))
					return 0
				end
			end
			return -99
		end

		def Internal.find_class(classname)
			# puts @@classes.keys.sort.join "\n"
			@@classes[classname]
		end
		
		# Runs the initializer as far as allocating the Qt C++ instance.
		# Then use a throw to jump back to here with the C++ instance 
		# wrapped in a new ruby variable of type T_DATA
		def Internal.try_initialize(instance, *args)
			initializer = instance.method(:initialize)
			catch "newqt" do
				initializer.call(*args)
			end
		end
		
        # If a block was passed to the constructor, then
		# run that now. Either run the context of the new instance
		# if no args were passed to the block. Or otherwise,
		# run the block in the context of the arg.
		def Internal.run_initializer_block(instance, block)
			if block.arity == -1
				instance.instance_eval(&block)
			elsif block.arity == 1
				block.call(instance)
			else
				raise ArgumentError, "Wrong number of arguments to block(#{block.arity} for 1)"
			end
		end

		def Internal.do_method_missing(package, method, klass, this, *args)
			if klass.class == Module
				classname = klass.name
			else
				classname = @@cpp_names[klass.name]
				if classname.nil?
					if klass != Object and klass != Qt
						return do_method_missing(package, method, klass.superclass, this, *args)
					else
						return nil
					end
				end
			end
			
			if method == "new"
				method = classname.dup 
				method.gsub!(/^(QTextLayout|KParts|KIO|KNS|DOM|Kontact|Kate|KTextEditor|KConfigSkeleton::ItemEnum|KConfigSkeleton|KWin)::/,"")
			end
			method = "operator" + method.sub("@","") if method !~ /[a-zA-Z]+/
			# Change foobar= to setFoobar()					
			method = 'set' + method[0,1].upcase + method[1,method.length].sub("=", "") if method =~ /.*[^-+%\/|=]=$/ && method != 'operator='

			methods = []
			methods << method.dup
			args.each do |arg|
				if arg.nil?
					# For each nil arg encountered, triple the number of munged method
					# templates, in order to cover all possible types that can match nil
					temp = []
					methods.collect! do |meth| 
						temp << meth + '?' 
						temp << meth + '#'
						meth << '$'
					end
					methods.concat(temp)
				elsif isObject(arg)
					methods.collect! { |meth| meth << '#' }
				elsif arg.kind_of? Array or arg.kind_of? Hash
					methods.collect! { |meth| meth << '?' }
				else
					methods.collect! { |meth| meth << '$' }
				end
			end
			
			methodIds = []
			methods.collect { |meth| methodIds.concat( findMethod(classname, meth) ) }
			
			if method =~ /_/ && methodIds.length == 0
				# If the method name contains underscores, convert to camel case
				# form and try again
				method.gsub!(/(.)_(.)/) {$1 + $2.upcase}
				return do_method_missing(package, method, klass, this, *args)
			end

			if debug_level >= DebugLevel::High
				puts "classname    == #{classname}"
				puts ":: method == #{method}"
				puts "-> methodIds == #{methodIds.inspect}"
				puts "candidate list:"
				prototypes = dumpCandidates(methodIds).split("\n")
				line_len = (prototypes.collect { |p| p.length }).max
				prototypes.zip(methodIds) { 
					|prototype,id| puts "#{prototype.ljust line_len}  (#{id})" 
				}
			end
			
			chosen = nil
			if methodIds.length > 0
				best_match = -1
				methodIds.each do
					|id|
					puts "matching => #{id}" if debug_level >= DebugLevel::High
					current_match = 0
					(0...args.length).each do
						|i|
						current_match += checkarg( getVALUEtype(args[i]), getTypeNameOfArg(id, i) )
					end
					
					# Note that if current_match > best_match, then chosen must be nil
					if current_match > best_match
						best_match = current_match
						chosen = id
					# Multiple matches are an error; the equality test below _cannot_ be commented out.
					# If ambiguous matches occur the problem must be fixed be adjusting the relative
					# ranking of the arg types involved in checkarg().
					elsif current_match == best_match
						chosen = nil
					end
					puts "match => #{id} score: #{current_match}" if debug_level >= DebugLevel::High
				end
					
				puts "Resolved to id: #{chosen}" if !chosen.nil? && debug_level >= DebugLevel::High
			end

			if debug_level >= DebugLevel::Minimal && chosen.nil? && method !~ /^operator/
				id = find_pclassid(normalize_classname(klass.name))
				hash = findAllMethods(id)
				constructor_names = nil
				if method == classname
					puts "No matching constructor found, possibles:\n"
					constructor_names = hash.keys.grep(/^#{classname}/)
				else
					puts "Possible prototypes:"
					constructor_names = hash.keys
				end
				method_ids = hash.values_at(*constructor_names).flatten
				puts dumpCandidates(method_ids)
			end

			puts "setCurrentMethod(#{chosen})" if debug_level >= DebugLevel::High
			setCurrentMethod(chosen) if chosen
			return nil
		end

		def Internal.init_all_classes()
			Qt::Internal::getClassList().each do |c|
				if c == "Qt"
					# Don't change Qt to Qt::t, just leave as is
					@@cpp_names["Qt"] = c
				elsif c != "QInternal"
					Qt::Internal::init_class(c)
				end
			end

			@@classes['Qt::Integer'] = Qt::Integer
			@@classes['Qt::Boolean'] = Qt::Boolean
			@@classes['Qt::Enum'] = Qt::Enum
		end
		
		def Internal.get_qinteger(num)
			return num.value
		end
		
		def Internal.set_qinteger(num, val)
			return num.value = val
		end
		
		def Internal.create_qenum(num, type)
			return Qt::Enum.new(num, type)
		end
		
		def Internal.get_qenum_type(e)
			return e.type
		end
		
		def Internal.get_qboolean(b)
			return b.value
		end
		
		def Internal.set_qboolean(b, val)
			return b.value = val
		end

		def Internal.getAllParents(class_id, res)
			getIsa(class_id).each do |s|
				c = idClass(s)
				res << c
				getAllParents(c, res)
			end
		end
	
		def Internal.signalInfo(qobject, signal_name)
			signals = Meta[qobject.class.name].get_signals
			signals.each_with_index do |signal, i|
				if signal.name == signal_name
					return [signal.reply_type, signal.full_name, i]
				end
			end
		end
	
		def Internal.getMocArguments(reply_type, member)
			argStr = member.sub(/.*\(/, '').sub(/\)$/, '')
			args = argStr.scan(/([^,]*<[^>]+>)|([^,]+)/)
			args.unshift reply_type
			mocargs = allocateMocArguments(args.length)
			args.each_with_index do |arg, i|
				arg = arg.to_s
				a = arg.sub(/^const\s+/, '')
				a = (a =~ /^(bool|int|double|char\*|QString)&?$/) ? $1 : 'ptr'
				valid = setMocType(mocargs, i, arg, a)
			end
			result = []
			result << args.length << mocargs
			result
		end

		#
		# From the enum MethodFlags in qt-copy/src/tools/moc/generator.cpp
		#
		AccessPrivate = 0x00
		AccessProtected = 0x01
		AccessPublic = 0x02
		MethodMethod = 0x00
		MethodSignal = 0x04
		MethodSlot = 0x08
		MethodCompatibility = 0x10
		MethodCloned = 0x20
		MethodScriptable = 0x40
	
		# Keeps a hash of strings against their corresponding offsets
		# within the qt_meta_stringdata sequence of null terminated
		# strings. Returns a proc to get an offset given a string.
		# That proc also adds new strings to the 'data' array, and updates 
		# the corresponding 'pack_str' Array#pack template.
		def Internal.string_table_handler(data, pack_str)
			hsh = {}
			offset = 0
			return lambda do |str|
				if !hsh.has_key? str
					hsh[str] = offset
					data << str
					pack_str << "a*x"
					offset += str.length + 1
				end

				return hsh[str]
			end
		end

		def Internal.makeMetaData(classname, classinfos, dbus, signals, slots)
			# Each entry in 'stringdata' corresponds to a string in the
			# qt_meta_stringdata_<classname> structure.
			# 'pack_string' is used to convert 'stringdata' into the
			# binary sequence of null terminated strings for the metaObject
			stringdata = []
			pack_string = ""
			string_table = string_table_handler(stringdata, pack_string)

			# This is used to create the array of uints that make up the
			# qt_meta_data_<classname> structure in the metaObject
			data = [1, 								# revision
					string_table.call(classname), 	# classname
					classinfos.length, classinfos.length > 0 ? 10 : 0, 	# classinfo
					signals.length + slots.length, 
					10 + (2*classinfos.length), 	# methods
					0, 0, 							# properties
					0, 0]							# enums/sets

			classinfos.each do |entry|
				data.push string_table.call(entry[0])		# key
				data.push string_table.call(entry[1])		# value
			end

			signals.each do |entry|
				data.push string_table.call(entry.full_name)				# signature
				data.push string_table.call(entry.full_name.delete("^,"))	# parameters
				data.push string_table.call(entry.reply_type)				# type, "" means void
				data.push string_table.call("")				# tag
				if dbus
					data.push MethodScriptable | MethodSignal | AccessPublic
				else
					data.push MethodSignal | AccessProtected	# flags, always protected for now
				end
			end

			slots.each do |entry|
				data.push string_table.call(entry.full_name)				# signature
				data.push string_table.call(entry.full_name.delete("^,"))	# parameters
				data.push string_table.call(entry.reply_type)				# type, "" means void
				data.push string_table.call("")				# tag
				if dbus
					data.push MethodScriptable | MethodSlot | AccessPublic	# flags, always public for now
				else
					data.push MethodSlot | AccessPublic		# flags, always public for now
				end
			end

			data.push 0		# eod

			return [stringdata.pack(pack_string), data]
		end
		
		def Internal.getMetaObject(klass, qobject)
			if klass.nil?
				klass = qobject.class
			end

			parentMeta = nil
			if @@cpp_names[klass.superclass.name].nil?
				parentMeta = getMetaObject(klass.superclass, qobject)
			end

			meta = Meta[klass.name]
			if meta.nil?
				meta = Qt::MetaInfo.new(klass) 
			end

			if meta.metaobject.nil? or meta.changed
				stringdata, data = makeMetaData(	qobject.class.name,
													meta.classinfos,  
													meta.dbus,
													meta.signals, 
													meta.slots )
				meta.metaobject = make_metaObject(qobject, parentMeta, stringdata, data)
				meta.changed = false
			end
			
			meta.metaobject
		end

		def Internal.connect(src, signal, target, block)
			args = (signal =~ /\((.*)\)/) ? $1 : ""
			signature = Qt::MetaObject.normalizedSignature("invoke(%s)" % args).to_s
			return Qt::Object.connect(	src,
										signal,
										Qt::BlockInvocation.new(target, block, signature),
										SLOT(signature) )
		end

		def Internal.signal_connect(src, signal, block)
			args = (signal =~ /\((.*)\)/) ? $1 : ""
			signature = Qt::MetaObject.normalizedSignature("invoke(%s)" % args).to_s
			return Qt::Object.connect(	src,
										signal,
										Qt::SignalBlockInvocation.new(src, block, signature),
										SLOT(signature) )
		end
	end # Qt::Internal

	Meta = {}
	
	# An entry for each signal or slot
	# Example 
	#  int foobar(QString,bool)
	#  :name is 'foobar'
	#  :full_name is 'foobar(QString,bool)'
	#  :arg_types is 'QString,bool'
	#  :reply_type is 'int'
	QObjectMember = Struct.new :name, :full_name, :arg_types, :reply_type

	class MetaInfo
		attr_accessor :classinfos, :dbus, :signals, :slots, :metaobject, :mocargs, :changed

		def initialize(klass)
			Meta[klass.name] = self
			@klass = klass
			@metaobject = nil
			@signals = []
			@slots = []
			@classinfos = []
			@dbus = false
			@changed = false
			Internal.addMetaObjectMethods(klass)
		end
		
		def add_signals(signal_list)
			signal_names = []
			signal_list.each do |signal|
				if signal.kind_of? Symbol
					signal = signal.to_s + "()"
				end
				signal = Qt::MetaObject.normalizedSignature(signal).to_s
				if signal =~ /^(([\w,<>:]*)\s+)?([^\s]*)\((.*)\)/
					@signals.push QObjectMember.new($3, $3 + "(" + $4 + ")", $4, ($2 == 'void' || $2.nil?) ? "" : $2)
					signal_names << $3
				else
					qWarning( "#{@klass.name}: Invalid signal format: '#{signal}'" )
				end
			end
			Internal.addSignalMethods(@klass, signal_names)
		end
		
		# Return a list of signals, including inherited ones
		def get_signals
			all_signals = []
			current = @klass
			while current != Qt::Base
				meta = Meta[current.name]
				if !meta.nil?
					all_signals.concat meta.signals
				end
				current = current.superclass
			end
			return all_signals
		end
		
		def add_slots(slot_list)
			slot_list.each do |slot|
				if slot.kind_of? Symbol
					slot = slot.to_s + "()"
				end
				slot = Qt::MetaObject.normalizedSignature(slot).to_s
				if slot =~ /^(([\w,<>:]*)\s+)?([^\s]*)\((.*)\)/
					@slots.push QObjectMember.new($3, $3 + "(" + $4 + ")", $4, ($2 == 'void' || $2.nil?) ? "" : $2)
				else
					qWarning( "#{@klass.name}: Invalid slot format: '#{slot}'" )
				end
			end
		end

		def add_classinfo(key, value)
			@classinfos.push [key, value]
			if key == 'D-Bus Interface'
				@dbus = true
			end
		end
	end # Qt::MetaInfo

	# These values are from the enum WindowType in qnamespace.h.
	# Some of the names such as 'Qt::Dialog', clash with QtRuby 
	# class names. So add some constants here to use instead,
	# renamed with an ending of 'Type'.
	WidgetType = 0x00000000
	WindowType = 0x00000001
	DialogType = 0x00000002 | WindowType
	SheetType = 0x00000004 | WindowType
	DrawerType = 0x00000006 | WindowType
	PopupType = 0x00000008 | WindowType
	ToolType = 0x0000000a | WindowType
	ToolTipType = 0x0000000c | WindowType
	SplashScreenType = 0x0000000e | WindowType
	DesktopType = 0x00000010 | WindowType
	SubWindowType =  0x00000012
		
end # Qt

class Object
	def SIGNAL(signal)
		if signal.kind_of? Symbol
			return "2" + signal.to_s + "()"
		else
			return "2" + signal
		end
	end

	def SLOT(slot)
		if slot.kind_of? Symbol
			return "1" + slot.to_s + "()"
		else
			return "1" + slot
		end
	end

	def emit(signal)
		return signal
	end

	def QT_TR_NOOP(x) x end
	def QT_TRANSLATE_NOOP(scope, x) x end

	# See the discussion here: http://eigenclass.org/hiki.rb?instance_exec
	# about implementations of the ruby 1.9 method instance_exec(). This
	# version is the one from Rails. It isn't thread safe, but that doesn't
	# matter for the intended use in invoking blocks as Qt slots.
	def instance_exec(*arguments, &block)
		block.bind(self)[*arguments]
	end
end

class Proc 
	# Part of the Rails Object#instance_exec implementation
	def bind(object)
		block, time = self, Time.now
		(class << object; self end).class_eval do
			method_name = "__bind_#{time.to_i}_#{time.usec}"
			define_method(method_name, &block)
			method = instance_method(method_name)
			remove_method(method_name)
			method
		end.bind(object)
	end
end

class Module
	alias_method :_constants, :constants
	alias_method :_instance_methods, :instance_methods
	alias_method :_protected_instance_methods, :protected_instance_methods
	alias_method :_public_instance_methods, :public_instance_methods

	private :_constants, :_instance_methods
	private :_protected_instance_methods, :_public_instance_methods

	def constants
		qt_methods(_constants, 0x10, true)
	end

	def instance_methods(inc_super=true)
		qt_methods(_instance_methods(inc_super), 0x0, inc_super)
	end

	def protected_instance_methods(inc_super=true)
		qt_methods(_protected_instance_methods(inc_super), 0x80, inc_super)
	end

	def public_instance_methods(inc_super=true)
		qt_methods(_public_instance_methods(inc_super), 0x0, inc_super)
	end

	private
	def qt_methods(meths, flags, inc_super=true)
		if !self.kind_of? Class
			return meths
		end

		klass = self
		classid = 0
		loop do
			classid = Qt::Internal::find_pclassid(klass.name)
			break if classid > 0
			
			klass = klass.superclass
			if klass.nil?
				return meths
			end
		end

		# These methods are all defined in Qt::Base, even if they aren't supported by a particular
		# subclass, so remove them to avoid confusion
		meths -= ["%", "&", "*", "**", "+", "-", "-@", "/", "<", "<<", "<=", ">", ">=", ">>", "|", "~", "^"]
		ids = []
		if inc_super
			Qt::Internal::getAllParents(classid, ids)
		end
		ids << classid
		ids.each { |c| Qt::Internal::findAllMethodNames(meths, c, flags) }
		return meths.uniq
	end
end
