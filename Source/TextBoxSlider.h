#ifndef _TEXTBOXSLIDER_H_
#define _TEXTBOXSLIDER_H_

#include "JuceHeader.h"


//===============================================================
// CLASS: TextBoxSlider
// Originally created by Haydxn
// Updated April 1st, 2011 by Jordan Hochenbaum
// original source can be found here http://www.anticore.org/juce/#
//---------------------------------------------------------------

/**	Simple widget to allow for slider-type control over a small
	text-box field.

	This widget is designed to save space, and provide a very
	simple yet effective means of specifying a number value.

	The number can be typed directly into the field, or the mouse
	can be used to 'drag' the number up or down like a slider. The
	cursor will change to an 'up/down' arrow to indicate that the
	behaviour is possible to the user.

	NOTE:	This class was built by taking the required members in
			the JUCE::Slider component, as of juce v1.12, and putting
			them back together in the appropriate manner inside a
			label component for the purposes of this widget.

	//TODO:	tidy it up!
*/
//===============================================================

class TextBoxSlider	:	public Label
{
private:
	/** The numerical parameters of the control */
	double	currentValue, minimum, maximum, interval;
	double	valueWhenLastDragged;
	/** The 'behind the scenes' integer parameters... */
	int numDecimalPlaces, mouseXWhenLastDragged, mouseYWhenLastDragged;
	int	sliderRegionStart, sliderRegionSize;

	bool	editableText	:	1;	// indicate editable status
	bool	isVelocityBased	:	1;	// velocity based sliding

public:
	TextBoxSlider(const String &name)
		:	Label( name, "0" ),
			currentValue(0),
			minimum(20.0),
			maximum(999.0),
			interval(1.0),
			numDecimalPlaces(2),
		    sliderRegionStart (0),
			sliderRegionSize (1),
			isVelocityBased(0)
	{
		// create the component

		setRepaintsOnMouseActivity( true );

		setEditable (editableText && isEnabled() );
		setEnabled(true);
		sliderRegionStart = 0;
        sliderRegionSize = 100; 
	}

	//==============================================================================

	double snapValue (double attemptedValue, const bool)
	{
	    return attemptedValue;
	}

	void setValue (double newValue,
						const bool sendUpdateMessage = false,
						const bool sendMessageSynchronously = false)
	{
		if (newValue <= minimum || maximum <= minimum)
		{
			newValue = minimum;
		}
		else if (newValue >= maximum)
		{
			newValue = maximum;
		}
		else if (interval > 0)
		{
			newValue = minimum + interval * floor ((newValue - minimum) / interval + 0.5);
		}

		if (currentValue != newValue)
		{
			hideEditor (true);
			currentValue = newValue;
			updateText();
			repaint();
		}
	}

	double getValue()
	{
		return currentValue;
	}

	void setRange (const double newMin,
						const double newMax,
						const double newInt)
	{
		if (minimum != newMin
			|| maximum != newMax
			|| interval != newInt)
		{
			minimum = newMin;
			maximum = newMax;
			interval = newInt;

			// figure out the number of DPs needed to display all values at this
			// interval setting.
			numDecimalPlaces = 7;

			if (newInt != 0)
			{
				int v = abs ((int) (newInt * 10000000));

				while ((v % 10) == 0)
				{
					--numDecimalPlaces;
					v /= 10;
				}
			}

			setValue (currentValue, false, false);
			updateText();
		}
	}
	
	//==============================================================================

	void startedDragging()
	{
	}

	void stoppedDragging()
	{
	}

	void valueChanged (double)
	{
	}

	//==============================================================================

	const String getTextFromValue (double v)
	{
		if (numDecimalPlaces > 0)
			return String (v, numDecimalPlaces);
		else
			return String (roundDoubleToInt (v));
	}

	double getValueFromText (const String& text)
	{
		return text.trimStart()
				.initialSectionContainingOnly ("0123456789.-")
				.getDoubleValue();
	}

	double proportionOfLengthToValue (double proportion)
	{
		return minimum + (maximum - minimum) * proportion;
	}

	double valueToProportionOfLength (double value)
	{
		const double n = (value - minimum) / (maximum - minimum);
		return n;
	}

	//==============================================================================


	void updateText()
	{
	    setText (getTextFromValue (currentValue), sendNotification);
	}

	void textChange (const bool retPressed,
					 const bool escPressed)
	{
		if (1)
		{
			if (retPressed || ! hasKeyboardFocus (true))
			{
				startedDragging();
				setValue (snapValue (getValueFromText (getText()), false), true, true);
				stoppedDragging();
			}

			if ((retPressed || escPressed) && hasKeyboardFocus (true))
			{
				grabKeyboardFocus();
				updateText();
			}
		}
	}

	void textEditorTextChanged (TextEditor& editor)
	{
		textChange (false, false);
	}

	void textEditorReturnKeyPressed (TextEditor& editor)
	{
		hideEditor(false);
		textChange (true, false);

	}

	void textEditorEscapeKeyPressed (TextEditor& editor)
	{
		hideEditor(true);
		textChange (false, true);
	}

	void textEditorFocusLost (TextEditor& editor)
	{
		hideEditor(false);
		textChange (false, true);
	}



	//==============================================================================
	void mouseUp (const MouseEvent&)
	{
		if ( isEnabled()	&&	(maximum > minimum) )
		{
			stoppedDragging();
		}
	}

	void mouseDown (const MouseEvent& e)
	{
		if ( isEnabled()	&&	(maximum > minimum) )
		{
			e.source.enableUnboundedMouseMovement(true);
			hideEditor (true);

			mouseXWhenLastDragged = e.x;
			mouseYWhenLastDragged = e.y;

			valueWhenLastDragged = currentValue;
			startedDragging();
		}
	}

	void mouseDrag (const MouseEvent& e)
	{
		if ( isEnabled()	&&	(maximum > minimum) )
		{
			// get the mouse y-axis position
			// int mousePos = e.y;
			// get the differentiated mouse movement amount...
			int mouseDiff = e.y - mouseYWhenLastDragged;

			const double maxSpeed = sliderRegionSize;
			double speed = jlimit (-maxSpeed, maxSpeed, (double) mouseDiff);

			if (speed != 0)
			{
				speed = jmax (0.0, speed * speed - 2.0) / 2;

				double delta = 0.5 * interval
								+ speed * (maximum - minimum) / (maxSpeed * maxSpeed);

				if (mouseDiff < 0)
					delta = -delta;

				// set sign for vertical action (y++ moves DOWN the screen)
				delta = -delta;

				valueWhenLastDragged += delta;
			}
		
			valueWhenLastDragged = jlimit (minimum, maximum, valueWhenLastDragged);
			setValue (snapValue (valueWhenLastDragged, true), true, false);
			mouseXWhenLastDragged = e.x;
			mouseYWhenLastDragged = e.y;
		}
	}


	void mouseWheelMove (const MouseEvent& e, float increment)
	{
		if (isEnabled()
			&& (maximum > minimum)
			&& ! isMouseButtonDownAnywhere())
		{
			hideEditor (false);

//			double delta = jmax ((maximum - minimum) * 0.04f * fabsf (increment),
//								interval);
			double delta = interval;

			if (increment < 0)
				delta = -delta;

			startedDragging();

			setValue (snapValue (currentValue + delta, false), true, true);

			stoppedDragging();
		}
	}
	void mouseDoubleClick(const MouseEvent& e)
	{
		showEditor();
	}
};

#endif//_TEXTBOXSLIDER_H_