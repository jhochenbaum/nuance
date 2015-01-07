/*
  ==============================================================================

   This file is part of the eJUCE library
   which is based on Raw Material Software ltd. JUCE

   eJUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   eJUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with eJUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
   Boston, MA 02111-1307 USA

   @author	Asnaghi Lucio

  ==============================================================================
*/

#ifndef __EJUCE_SERIALIZABLE__
#define __EJUCE_SERIALIZABLE__

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
template<class TypeName>
class Serializable
{
public:

	/** Virtual destructor */
	virtual ~Serializable () {}

	/** Subclass this for serialize an object in a XML element
	*/
	virtual TypeName* serialize () = 0;

	/** Subclass this for restore an object from a XML element
	*/
	virtual void restore (TypeName* e) = 0;
};


//==============================================================================
class SerializeFactoryXml
{
public:

	/** constructor */
	SerializeFactoryXml () {}

	/** Virtual destructor */
	~SerializeFactoryXml () { objects.clear(); }

	/** Register a serializable with this factory */
	void addSerializable (Serializable<XmlElement>* object);

	/** Removes a serializable object from the factory */
	void removeSerializable (Serializable<XmlElement>* object);

	/** Serialize all the objects registered */
	XmlElement* serializeObjects (const String& rootName);

	/** Restored the objects registered with the root element */
	void restoreObjects (XmlElement* root);


	juce_DeclareSingleton (SerializeFactoryXml, true)

protected:
	Array <Serializable<XmlElement>* > objects;
};



#endif   // __EJUCE_SERIALIZABLE__
