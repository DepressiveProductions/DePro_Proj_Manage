#include "Sound.h"

Sound::Sound(void){}
Sound::~Sound(void){}

int Sound::init()
{
	//Initialize OpenAL 
    //ALCdevice *device;														//Create an OpenAL Device
    //ALCcontext *context;														//And an OpenAL Context
    device = alcOpenDevice(NULL);                                               //Open the device
    if(!device) 
		return endWithError("no sound device");									//Error during device oening
    context = alcCreateContext(device, NULL);                                   //Give the device a context
    alcMakeContextCurrent(context);                                             //Make the context the current
    if(!context) 
		return endWithError("no sound context");								//Error during context handeling
}

int Sound::load(const char *szFileName)
{
	//Load WAVE file:
	//FILE *fp = NULL;																						//Create FILE pointer for the WAVE file
	fileNames.push_back(szFileName);
	fp[szFileName] = fopen(szFileName, "rb");																//Open the WAVE file
	if(!fp[szFileName])
		return endWithError("Failed to open file");															//Could not open file

	//Variables to store info about the WAVE file (all of them is not needed for OpenAL)
    char type[4];
    DWORD size,chunkSize;
    short formatType,channels;
    DWORD sampleRate,avgBytesPerSec;
    short bytesPerSample,bitsPerSample;
    DWORD dataSize;
    
    //Check that the WAVE file is OK
    fread(type,sizeof(char),4,fp[szFileName]);                                              //Reads the first bytes in the file
    if(type[0]!='R' || type[1]!='I' || type[2]!='F' || type[3]!='F')            //Should be "RIFF"
		return endWithError ("No RIFF");                                        //Not RIFF

    fread(&size, sizeof(DWORD),1,fp[szFileName]);                                           //Continue to read the file
    fread(type, sizeof(char),4,fp[szFileName]);                                             //Continue to read the file
    if (type[0]!='W' || type[1]!='A' || type[2]!='V' || type[3]!='E')           //This part should be "WAVE"
		return endWithError("not WAVE");                                        //Not WAVE
    
    fread(type,sizeof(char),4,fp[szFileName]);                                              //Continue to read the file
    if (type[0]!='f' || type[1]!='m' || type[2]!='t' || type[3]!=' ')           //This part should be "fmt "
		return endWithError("not fmt ");                                        //Not fmt 
    
    //Now we know that the file is a acceptable WAVE file
    //Info about the WAVE data is now read and stored
    fread(&chunkSize,sizeof(DWORD),1,fp[szFileName]);
    fread(&formatType,sizeof(short),1,fp[szFileName]);
    fread(&channels,sizeof(short),1,fp[szFileName]);
    fread(&sampleRate,sizeof(DWORD),1,fp[szFileName]);
    fread(&avgBytesPerSec,sizeof(DWORD),1,fp[szFileName]);
    fread(&bytesPerSample,sizeof(short),1,fp[szFileName]);
    fread(&bitsPerSample,sizeof(short),1,fp[szFileName]);
    
    fread(type,sizeof(char),4,fp[szFileName]);
    if (type[0]!='d' || type[1]!='a' || type[2]!='t' || type[3]!='a')           //This part should be "data"
		return endWithError("Missing DATA");                                    //not data
    
    fread(&dataSize,sizeof(DWORD),1,fp[szFileName]);                                        //The size of the sound data is read
    
    //Display the info about the WAVE file
    std::cout << "Chunk Size: " << chunkSize << std::endl;
    std::cout << "Format Type: " << formatType << std::endl;
    std::cout << "Channels: " << channels << std::endl;
    std::cout << "Sample Rate: " << sampleRate << std::endl;
    std::cout << "Average Bytes Per Second: " << avgBytesPerSec << std::endl;
    std::cout << "Bytes Per Sample: " << bytesPerSample << std::endl;
    std::cout << "Bits Per Sample: " << bitsPerSample << std::endl;
    std::cout << "Data Size: " << dataSize << std::endl;
        
    buf[szFileName] = new unsigned char[dataSize];															//Allocate memory for the sound data
    std::cout << fread(buf[szFileName], sizeof(BYTE), dataSize, fp[szFileName]) << " bytes loaded\n";		//Read the sound data and display the 
																											//number of bytes loaded.
																											//Should be the same as the Data Size if OK
    

    ALuint src;																	//Is the name of source (where the sound come from) nej
    ALuint bfr;																	//Stores the sound data nej
    ALuint frequency=sampleRate;												//The Sample Rate of the WAVE file
    ALenum format=0;                                                            //The audio format (bits per sample, number of channels)
    
    alGenBuffers(1, &bfr);                                                   //Generate one OpenAL Buffer and link to "buffer"
    alGenSources(1, &src);                                                   //Generate one OpenAL Source and link to "source"
    if(alGetError() != AL_NO_ERROR) 
		return endWithError("Error GenSource");									//Error during buffer/source generation
    
    //Figure out the format of the WAVE file
    if(bitsPerSample == 8)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO8;
        else if(channels == 2)
            format = AL_FORMAT_STEREO8;
    }
    else if(bitsPerSample == 16)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO16;
        else if(channels == 2)
            format = AL_FORMAT_STEREO16;
    }
    if(!format) 
		return endWithError("Wrong BitPerSample");								//Not valid format

	alBufferData(bfr, format, buf[szFileName], dataSize, frequency);						//Store the sound data in the OpenAL Buffer
    if(alGetError() != AL_NO_ERROR) 
		return endWithError("Error loading ALBuffer");                          //Error during buffer loading

	buffer[szFileName] = bfr;
	source[szFileName] = src;

	return 0;
}

int Sound::play(const char *key, bool loop, ALfloat sourcePos[], ALfloat sourceVel[])
{
	/*	//Sound setting variables
    ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };                                    //Position of the source sound
    ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };                                    //Velocity of the source sound	*/
    ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };                                  //Position of the listener
    ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };                                  //Velocity of the listener
    ALfloat listenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };                 //Orientation of the listener
                                                                                //First direction vector, then vector pointing up)	 
    //Listener                                                                               
    alListenerfv(AL_POSITION,    listenerPos);                                  //Set position of the listener
    alListenerfv(AL_VELOCITY,    listenerVel);                                  //Set velocity of the listener
    alListenerfv(AL_ORIENTATION, listenerOri);                                  //Set orientation of the listener
    
    //Source
    alSourcei (source[key], AL_BUFFER,   buffer[key]);								 //Link the buffer to the source
    alSourcef (source[key], AL_PITCH,    1.0f     );                                 //Set the pitch of the source
    alSourcef (source[key], AL_GAIN,     1.0f     );                                 //Set the gain of the source
    alSourcefv(source[key], AL_POSITION, sourcePos);                                 //Set the position of the source
    alSourcefv(source[key], AL_VELOCITY, sourceVel);                                 //Set the velocity of the source
	if (loop)
		alSourcei (source[key], AL_LOOPING,  AL_TRUE );								//Set if source is looping sound
	else
		alSourcei (source[key], AL_LOOPING, AL_FALSE );
    
    //PLAY 
    alSourcePlay(source[key]);                                                       //Play the sound buffer linked to the source
    if(alGetError() != AL_NO_ERROR) 
		return endWithError("Error playing sound");								//Error when playing sound
    //system("PAUSE");															//Pause to let the sound play;
}

void Sound::play(const char *key, bool loop, ALfloat sourcePos[])								//Default value for sourceVel
{
	ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };
	play(key, loop, sourcePos, sourceVel);
}

void Sound::play(const char *key, bool loop)														//Default value for sourcePos and sourceVel
{
	ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
	ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };
	play(key, loop, sourcePos, sourceVel);
}

void Sound::play(const char *key)																//Default value for sourcePos, sourceVel and loop
{
	bool loop = FALSE;
	ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
	ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };
	play(key, loop, sourcePos, sourceVel);
}

void Sound::newSpeed(const char *key, ALfloat sourceVel)
{

}

int Sound::endWithError(char* msg, int error)
{
    //Display error message in console
    std::cout << msg << std::endl;
    system("PAUSE");
    return error;
}

void Sound::clearSound()														//Clean-up
{
	for(int i = 0; i < fileNames.size(); i++)
	{
		fclose(fp[fileNames[i]]);												//Close the WAVE file
		alDeleteSources(1, &source[fileNames[i]]);								//Delete the OpenAL Source
		alDeleteBuffers(1, &buffer[fileNames[i]]);								//Delete the OpenAL Buffer
		delete[] buf[fileNames[i]];												//Delete the sound data buffer
	}
    alcMakeContextCurrent(NULL);												//Make no context current
    alcDestroyContext(context);													//Destroy the OpenAL Context
    alcCloseDevice(device);														//Close the OpenAL Device
}

/*
void Sound::clearSoundPartial()													//Clean-up
{
    fclose(fp);																	//Close the WAVE file
    //delete[] buf;																//Delete the sound data buffer
    //alDeleteSources(1, &source);												//Delete the OpenAL Source
    alDeleteBuffers(1, &buffer);												//Delete the OpenAL Buffer
    //alcMakeContextCurrent(NULL);												//Make no context current
    //alcDestroyContext(context);													//Destroy the OpenAL Context
    //alcCloseDevice(device);														//Close the OpenAL Device
}
*/