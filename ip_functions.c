/*
 * ip_functions.c
 *
 * Contains all functions which pertain to setup and use of IP periperals.
 */

#include "adventures_with_ip.h"
#include "audio.h"




/* ---------------------------------------------------------------------------- *
 * 								lms_filter()									*
 * ---------------------------------------------------------------------------- *
 * This function adds a tonal noise component to the sampled audio from the
 * audio codec by passing a step size to the input of an NCO component in the
 * PL. A sinusoidal signal is received back from the NCO which is then scaled
 * and added to the sampled audio. The audio + noise sample can then be
 * adaptively filtered using an LMS filter in the PL. The resulting audio,
 * filtered or not, is then output to the audio codec.
 *
 * The main menu can be accessed by entering 'q' on the keyboard.
 * ---------------------------------------------------------------------------- */
void lms_filter()
{
	u32 nco_in, nco_out, in_left, in_right, out_left, out_right, step, \
		prevL, prevR, prevTone, temp;

	/* Read step size value from DIP switches */
	step = XGpio_DiscreteRead(&Gpio, SWITCH_CHANNEL);

	/* Write step size value to the LEDs */
	Xil_Out32(LED_BASE, step);

	/* Scale the step size */
	nco_in = step;
	xil_printf("Step = %d, nco_in = %d\r\n",step, nco_in);

	while (!XUartPs_IsReceiveData(UART_BASEADDR)){

		/* Input step size to the NCO core */
		XNco_Set_step_size_V(&Nco, nco_in);

		/* Receive sinusoidal sample from NCO core */
		nco_out = XNco_Get_sine_sample_V(&Nco);

		if(nco_out!=prevTone) { /* New sinusoidal sample? */
			temp = nco_out;
		}

		/* Sample L+R audio from the codec */

		in_left = Xil_In32(I2S_DATA_RX_L_REG);
		in_right = Xil_In32(I2S_DATA_RX_R_REG);

		/* -------------------------------------------------------------------------------- *
		 * --------------------------------- LEFT CHANNEL --------------------------------- *
		 * -------------------------------------------------------------------------------- */
		if(in_left != prevL) /* New left sample? */
		{
			/* Add noise component to the L+R audio samples */
			out_left = (temp + in_left);

			Xil_Out32(LMS_D, out_left >> SCALE);	// Input audio+noise as desired signal
			Xil_Out32(LMS_X, temp >> SCALE);		// Input noise as input
			Xil_Out32(LMS_STROBE, 0x01);			// Stobe LMS to signal inputs are finished

			/* If any button is pressed */
			if(XGpio_DiscreteRead(&Gpio, BUTTON_CHANNEL)>0){

				/* Wait until output data is ready */
				out_left = (Xil_In32(LMS_E) << (SCALE-1)); // Output filtered audio
			}

			/* Output audio to the codec */
			Xil_Out32(I2S_DATA_TX_L_REG, out_left);

		}

		/* -------------------------------------------------------------------------------- *
		 * --------------------------------- RIGHT CHANNEL -------------------------------- *
		 * -------------------------------------------------------------------------------- */
		if(in_right != prevR) /* New right sample? */
		{
			/* Add scaled noise component to the L+R audio samples */
			out_right = (temp + in_right);

			Xil_Out32(LMS_D, out_right >> SCALE); 	// Input audio+noise as desired signal
			Xil_Out32(LMS_X, temp >> SCALE); 		// Input noise as input
			Xil_Out32(LMS_STROBE, 0x01);			// Stobe LMS to signal inputs are finished

			/* If any button is pressed */
			if(XGpio_DiscreteRead(&Gpio, BUTTON_CHANNEL)>0){
				out_right = (Xil_In32(LMS_E) << (SCALE-1)); // output filtered audio
			}

			/* Output audio to the codec */
			Xil_Out32(I2S_DATA_TX_R_REG, out_right);
		}

		/* Update previous input values */
		prevL = in_left;
		prevR = in_right;
		prevTone = nco_out;

		/* If the DIP switch values have changed, break from while
		 * loop to allow the step size value to update.
		 */
		if(step != XGpio_DiscreteRead(&Gpio, SWITCH_CHANNEL)) break;
	} // while
	/* If input from the terminal is 'q', then return to menu.
	 * Else, continue. */
	if(XUartPs_ReadReg(UART_BASEADDR, XUARTPS_FIFO_OFFSET)=='q') menu();
	else lms_filter();

} // LMS filtering

/* ---------------------------------------------------------------------------- *
 * 								tonal_noise()									*
 * ---------------------------------------------------------------------------- *
 * This function adds a tonal noise component to the sampled audio from the
 * audio codec by passing a step size to the input of an NCO component in the
 * PL. A sinusoidal signal is received back from the NCO which is then scaled
 * and added to the sampled audio. The audio + noise sample is then sent to
 * the audio codec for output.
 *
 * The main menu can be accessed by entering 'q' on the keyboard.
 * ---------------------------------------------------------------------------- */
void tonal_noise(void)
{
	u32 nco_in, nco_out, in_left, in_right, out_left, out_right, step, temp, input_l,input_r;

	/* Read step size value from DIP switches */
	step = XGpio_DiscreteRead(&Gpio, SWITCH_CHANNEL);

	/* Write step size value to the LEDs */
	Xil_Out32(LED_BASE, step);

	/* Scale the step size */
	nco_in = step;

	xil_printf("Step = %d, nco_in = %d\r\n",step, nco_in);

	while (!XUartPs_IsReceiveData(UART_BASEADDR)){

		/* Input scaled step size to the NCO core */
		XNco_Set_step_size_V(&Nco, nco_in);

		/* Receive sinusoidal sample from NCO core */
		nco_out = XNco_Get_sine_sample_V(&Nco);

		temp = nco_out;

		/* Sample L+R audio from the codec */
		/* Sampling rate is 48 kHz*/
		int i,j;
		in_left = 0;
		in_right = 0;
		int samples = 0;
		if (step == 1){
			samples = 200;
		} else if (step == 2){
			samples = 400;
		} else if (step == 3){
			samples = 600;
		} else if (step == 4){
			samples = 800;
		} else if (step == 5){
			samples = 1000;
		} else if (step == 6){
			samples = 1200;
		} else if (step == 7){
			samples = 1400;
		} else if (step == 8){
			samples = 1600;
		} else if (step == 9){
			samples = 1800;
		} else {
			samples = 0;
		}
		/* switch(step){
			case 1: samples = 240; //play sound wave with frequency of 200 Hz
			case 2: samples = 160; //play sound wave with frequency of 300 Hz
			case 3: samples = 120; //play sound wave with frequency of 400 Hz
			case 4: samples = 96; //play sound wave with frequency of 500 Hz
			case 5: samples = 80; //play sound wave with frequency of 600 Hz
			case 6: samples = 69; //play sound wave with frequency of 700 Hz
			case 7: samples = 60; //play sound wave with frequency of 800 Hz
			case 8: samples = 53; //play sound wave with frequency of 900 Hz
			case 9: samples = 48; //play sound wave with frequency of 1 kHz
		}
		*/


		for (i = 1; i < 10; i++){
			for (j = 0; j < samples/2; j++){
				Xil_Out32(I2S_DATA_TX_L_REG, in_left);
				Xil_Out32(I2S_DATA_TX_R_REG, in_right);
			}
			in_left = 8388607; //set HIGH = 2^(23) - 1
			in_right = 8388607; //set HIGH = 2^(23) - 1
			for (j = 1; j < samples/2; j++){
					Xil_Out32(I2S_DATA_TX_L_REG, in_left);
					Xil_Out32(I2S_DATA_TX_R_REG, in_right);
			}
			in_left = 8388608; //set LOW = 2^23
			in_right = 8388608; //set LOW = 2^23
		}

		/* Add scaled noise component to the L+R audio samples */
		//out_left =  temp + in_left;
		//out_right = temp + in_right;

		/* Output corrupted audio to the codec */
		//Xil_Out32(I2S_DATA_TX_L_REG, in_left);
		//Xil_Out32(I2S_DATA_TX_R_REG, in_right);

		/* If the DIP switch values have changed, break from while
		 * loop to allow the step size value to update.
		 */
		if(step != XGpio_DiscreteRead(&Gpio, SWITCH_CHANNEL)) break;
	} // while
	/* If input from the terminal is 'q', then return to menu.
	 * Else, continue. */
	if(XUartPs_ReadReg(UART_BASEADDR, XUARTPS_FIFO_OFFSET) == 'q') menu();
	else tonal_noise();

} // tonal_noise()

/* ---------------------------------------------------------------------------- *
 * 								audio_stream()									*
 * ---------------------------------------------------------------------------- *
 * This function performs audio loopback streaming by sampling the input audio
 * from the codec and then immediately passing the sample to the output of the
 * codec.
 *
 * The main menu can be accessed by entering 'q' on the keyboard.
 * ---------------------------------------------------------------------------- */
void audio_stream(){

	//int index = 0;
	u32  in_left, in_right;

	while (!XUartPs_IsReceiveData(UART_BASEADDR)){

		// Read audio input from codec
		in_left = Xil_In32(I2S_DATA_RX_L_REG);
		in_right = Xil_In32(I2S_DATA_RX_R_REG);

		// Write audio input to codec
		Xil_Out32(I2S_DATA_TX_L_REG, in_left);
		Xil_Out32(I2S_DATA_TX_R_REG, in_right);

	}

	/* If input from the terminal is 'q', then return to menu.
	 * Else, continue streaming. */
	if(XUartPs_ReadReg(UART_BASEADDR, XUARTPS_FIFO_OFFSET) == 'q') menu();
	else audio_stream();
} // audio_stream()


/* ---------------------------------------------------------------------------- *
 * 								gpio_initi()									*
 * ---------------------------------------------------------------------------- *
 * Initialises the GPIO driver for the push buttons and switches.
 * ---------------------------------------------------------------------------- */
unsigned char gpio_init()
{
	int Status;

	Status = XGpio_Initialize(&Gpio, BUTTON_SWITCH_ID);
	if(Status != XST_SUCCESS) return XST_FAILURE;
	Status = XGpio_Initialize(&Gpio_audio_enable, AUDIO_ENABLE_ID);
	if(Status != XST_SUCCESS) return XST_FAILURE;

	XGpio_SetDataDirection(&Gpio_audio_enable, 1, 0x00);
	XGpio_SetDataDirection(&Gpio, SWITCH_CHANNEL, 0xFF);
	XGpio_SetDataDirection(&Gpio, BUTTON_CHANNEL, 0xFF);

	return XST_SUCCESS;
}

/* ---------------------------------------------------------------------------- *
 * 								nco_initi()									*
 * ---------------------------------------------------------------------------- *
 * Initialises the NCO driver by looking up the configuration in the config
 * table and then initialising it.
 * ---------------------------------------------------------------------------- */
void nco_init(void *InstancePtr){
	XNco_Config *cfgPtr;
	int status;

	/* Initialise the NCO driver so that it's ready to use */

	// Look up the configuration in the config table
	cfgPtr = XNco_LookupConfig(NCO_ID);
	if (!cfgPtr) {
		print("ERROR: Lookup of NCO configuration failed.\n\r");
	}

	// Initialise the NCO driver configuration
	status = XNco_CfgInitialize(InstancePtr, cfgPtr);
	if (status != XST_SUCCESS) {
		print("ERROR: Could not initialise NCO.\n\r");
	}
}
