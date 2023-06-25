IMPORTANT NOTES:
> To install unzip in your Unreal Engine 4 Projects Directory and run the .uproject file
> For best practice make sure that all inputs in the "WS_Texture_4WayChaos" match across the surface and post-process materials used.
> For additional info please look in the "Instructions" file for details on what different parameters do.
> Feel free to look at or directly use any of the pre-made material instances!
> For best visuals, the current presets does not use "Screen Space Reflections" by default - these can be enabled though the MasterMaterials.

Known Bugs: 
> The water surface does not always show light reflection accurately. This seems to be an issue with the forward shading transparency in UE4 
	- to resolve this cycle through the "Engine Scalability Setting" until the light reflections appear. 
> Upon launch the effects do not display properly in the editor 
	- to fix enter and exit full-screen once.

Changelog:
> Fixed windowed look for all post-prcess effects. The effects should now scale accurately across all resolutions and aspect ratios.
> Removed "Line Blur Samples" from the "UnderWaterBoxBlur" - the number of blur samples are now unversal for the whole shader.
> Added Shared Parameter for Screen Percentage Offset: R and G values can be used to correct the screen-space post-process effects in windowed mode or during Screen Percentage changes in the editor viewport. 
		NOTE: Default Values are R = 0.52 G = 96 or R = 0.26 G = 48 for corrected Screen%


External Credits:
> Big thanks to Unreal forums user "expose" for making his HSLS blurs availabnle to the community!
His work contributed massively to the final visuals of this project!
> Massive shout out to Unreal forums user "duke22" for making some awesome normal textures availabnle to the community!
These made the surface shader look awesome!
> Big thanks to HDRI Heaven for making some of the best HDRIs I have seen for free to use in this project!
> Mega thanks to ELHOUSSINE  MEHNIK at ue4resources.com for the awesome tutorials that made Waterline all the better!
> Huge thanks to Nitrogen a Youtube channel full of awesome help and tutorials!
> Huge thanks to Fynn Flügge who explained FFT Oceans in his youtube channel!
> Huge thanks to Yivan Lee for his breakdown of Oncean FFT effects in gaming!
> Huge thanks to Mark Serena for his curvature function tutorial!
> Huge thanks to Jan Arenz for his tutorial on texture variations! 

TRY THE DEMO AT:
https://1drv.ms/u/s!Ahv0CVYoiZNyggqEfuMJJVSplFdJ

Legal: 
You may use this content for any commercial or personal project.
This is a single user liscence so you may not distribute/resell this asset pack to other people.
