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

//BEGIN_EJUCE_NAMESPACE

#include "Serializable.h"


//==============================================================================
void SerializeFactoryXml::addSerializable (Serializable<XmlElement>* object)
{
	objects.addIfNotAlreadyThere (object);
}

void SerializeFactoryXml::removeSerializable (Serializable<XmlElement>* object)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects.getUnchecked(i) == object)
		{
			objects.remove (i);
			break;
		}
	}
}

//==============================================================================
XmlElement* SerializeFactoryXml::serializeObjects (const String& rootName)
{
	XmlElement* root = new XmlElement (rootName);

	for (int i = 0; i < objects.size(); ++i)
	{
		root->addChildElement (objects.getUnchecked(i)->serialize ());
	}

	return root;
}

void SerializeFactoryXml::restoreObjects (XmlElement* root)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		objects.getUnchecked(i)->restore (root);
	}
}

//==============================================================================
juce_ImplementSingleton (SerializeFactoryXml)


//END_EJUCE_NAMESPACE
