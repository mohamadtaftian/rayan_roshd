void Viewer_L1()
{
	try
	{

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GL_TEXTURE_STORAGE_HINT_APPLE, GL_FASTEST);
		window1 = glfwCreateWindow(window_width, window_height, "Simple example", NULL, NULL);
		if (!window1)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwSetKeyCallback(window1, key_callback_L1);
		glfwSetWindowSizeCallback(window1, resize_callback_L1);
		glfwMakeContextCurrent(window1);
		glfwSwapInterval(1);
		//  Initialise glew (must occur AFTER window1 creation or glew will error)
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			//cout << "GLEW initialisation error: " << glewGetErrorString(err) << endl;
			exit(-1);
		}
		//cout << "GLEW okay - using version: " << glewGetString(GLEW_VERSION) << endl;
		init_opengl_L1(window_width, window_height);
		glfwSetWindowPos(window1, win_x_offset, win_y_offset);

		LARGE_INTEGER frequency;
		LARGE_INTEGER t1, t2, t20;
		double elapsedTime, elapsedTime10;

		//################################################################################### AA
		glfwSwapInterval(1);
		glEnable(GL_TEXTURE_2D);
		GLuint tex;
		glGenTextures(1, &tex);
		//################################################################################### BB
		glTexImage2D(
			GL_TEXTURE_2D,     // Type of texture
			0,                 // Pyramid level (for mip-mapping) - 0 is the top level
			GL_RGB,            // Internal colour format to convert to
			view_small_L1.cols,   // Image width  i.e. 640 for Kinect in standard mode
			view_small_L1.rows,   // Image height i.e. 480 for Kinect in standard mode
			0,                 // Border width in pixels (can either be 1 or 0)
			GL_BGR,            // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
			GL_UNSIGNED_BYTE,  // Image data type
			view_small_L1.ptr()); // The actual image data itself

		glGenerateMipmap(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glTexCoord2i(0, 1); glVertex2i(0, window_height);
		glTexCoord2i(1, 1); glVertex2i(window_width, window_height);
		glTexCoord2i(1, 0); glVertex2i(window_width, 0);

		glEnd();

		glfwSwapBuffers(window1);
		glfwPollEvents();
		//condVar7.notify_one();
		//###############################################################################################

		while (!glfwWindowShouldClose(window1))
		{
			try
			{
				glTexImage2D(
					GL_TEXTURE_2D,     // Type of texture
					0,                 // Pyramid level (for mip-mapping) - 0 is the top level
					GL_RGB,            // Internal colour format to convert to
					view_small_L1.cols,          // Image width  i.e. 640 for Kinect in standard mode
					view_small_L1.rows,          // Image height i.e. 480 for Kinect in standard mode
					0,                 // Border width in pixels (can either be 1 or 0)
					GL_BGR, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
					GL_UNSIGNED_BYTE,  // Image data type
					view_small_L1.ptr());        // The actual image data itself

				glGenerateMipmap(GL_TEXTURE_2D);

				glBegin(GL_QUADS);
				glTexCoord2i(0, 0); glVertex2i(0, 0);
				glTexCoord2i(0, 1); glVertex2i(0, window_height);
				glTexCoord2i(1, 1); glVertex2i(window_width, window_height);
				glTexCoord2i(1, 0); glVertex2i(window_width, 0);

				glEnd();

				glfwSwapBuffers(window1);
				glfwPollEvents();


			}
			catch (const std::exception& e)
			{
				logging::INFO("error // view_internal_scope");
			}
		}
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // viewer 1 ");
		logging::INFO(e.what());
	}

}
void Update_FB_Seeker()
{
	int Last_Correct_Object_index = Find_Last_Written_Object_Index();


	bool decreased_range = Should_decreased_Range();


	if (OBJCNT_OUT >= Max_Objects_FB)
	{
		std::vector<int>index_edition_vec;

		for (int i = 0; i < Max_Objects_FB; i++)
		{
			index_edition_vec.push_back(i);
		}

		int offset_sum_1 = Max_Objects_FB - 1 - Last_Correct_Object_index;

		for (int i = 0; i < Max_Objects_FB; i++)
		{
			index_edition_vec[i] += offset_sum_1;
		}

		for (int i = 0; i < Max_Objects_FB; i++)
		{
			if (index_edition_vec[i] > Max_Objects_FB - 1)
			{
				index_edition_vec[i] -= Max_Objects_FB;
			}
			else
			{
				//IGNORE
			}
		}

		Prioritized_Index_Vec.clear();

		Prioritized_Index_Vec = sort_indexes(index_edition_vec);
		// Reverse the vector
		//reverse(Prioritized_Index_Vec.begin(), Prioritized_Index_Vec.end());

		if (decreased_range)
		{
			L1L2_FB_Mode_Up_Range_Index = Max_Objects_FB - 1;
			L1L2_FB_Mode_Low_Range_Index = 1;
			number_of_frame = Max_Objects_FB - 1;
		}
		else
		{
			L1L2_FB_Mode_Up_Range_Index = Max_Objects_FB - 1;
			L1L2_FB_Mode_Low_Range_Index = 0;
			number_of_frame = Max_Objects_FB;
		}
		logging::INFO("L1 >> Last_Correct_Object_index: " + to_string(Last_Correct_Object_index));
		logging::INFO("L1 >> decreased_range: " + to_string(decreased_range));
		logging::INFO("L1 >> OBJCNT_OUT: " + to_string(OBJCNT_OUT));
		logging::INFO("L1 >> L1L2_FB_Mode_Up_Range_Index: " + to_string(L1L2_FB_Mode_Up_Range_Index));
		logging::INFO("L1 >> L1L2_FB_Mode_Low_Range_Index: " + to_string(L1L2_FB_Mode_Low_Range_Index));
		logging::INFO("L1 >> number_of_frame: " + to_string(number_of_frame));

		logging::INFO("L2 >> Last_Correct_Object_index: " + to_string(Last_Correct_Object_index));
		logging::INFO("L2 >> decreased_range: " + to_string(decreased_range));
		logging::INFO("L2 >> OBJCNT_OUT: " + to_string(OBJCNT_OUT));
		logging::INFO("L2 >> L1L2_FB_Mode_Up_Range_Index: " + to_string(L1L2_FB_Mode_Up_Range_Index));
		logging::INFO("L2 >> L1L2_FB_Mode_Low_Range_Index: " + to_string(L1L2_FB_Mode_Low_Range_Index));
		logging::INFO("L2 >> number_of_frame: " + to_string(number_of_frame));
	}
	else
	{

		if (decreased_range)
		{
			if (L1_OPP > L2_OPP)
			{
				L1L2_FB_Mode_Up_Range_Index = L2_OPP;
			}
			else
			{
				L1L2_FB_Mode_Up_Range_Index = L1_OPP;
			}
			L1L2_FB_Mode_Low_Range_Index = 0;
			number_of_frame = L1L2_FB_Mode_Up_Range_Index + 1;
		}
		else
		{
			L1L2_FB_Mode_Up_Range_Index = L1_OPP;
			L1L2_FB_Mode_Low_Range_Index = 0;
			number_of_frame = L1L2_FB_Mode_Up_Range_Index + 1;
		}

		Prioritized_Index_Vec.clear();

		for (int k = 0; k < number_of_frame; k++)
		{
			Prioritized_Index_Vec.push_back(k);
		}
		// Reverse the vector
		//reverse(Prioritized_Index_Vec.begin(), Prioritized_Index_Vec.end());
		logging::INFO("L1 >> L1L2_FB_Mode_Up_Range_Index: " + to_string(L1L2_FB_Mode_Up_Range_Index));
		logging::INFO("L1 >> L1L2_FB_Mode_Low_Range_Index: " + to_string(L1L2_FB_Mode_Low_Range_Index));
		logging::INFO("L1 >> number_of_frame: " + to_string(number_of_frame));

		logging::INFO("L2 >> L1L2_FB_Mode_Up_Range_Index: " + to_string(L1L2_FB_Mode_Up_Range_Index));
		logging::INFO("L2 >> L1L2_FB_Mode_Low_Range_Index: " + to_string(L1L2_FB_Mode_Low_Range_Index));
		logging::INFO("L2 >> number_of_frame: " + to_string(number_of_frame));

	}

	L1L2_Seek_FB = L1L2_FB_Mode_Up_Range_Index;

}
void PreCommand()
{
	//logging::INFO("big_index: " + to_string(big_index));
	//logging::INFO("==============================================================================================");

	//logging::INFO("precommmand called");

	string gg;
	try
	{
		if (!M_OTS)
		{
			/////////////////////////////////////////////////////FOR BIG_RECT_FAST
			if (!L1_Full_Big_Buffer)
			{
				default_big_for_rectangle_L1(cv::Rect(0, 0, BIG_WIN_WIDTH, WIN_HEIGHT)).copyTo(default_big_for_rectangle_fast_L1(cv::Rect(0, 0, BIG_WIN_WIDTH, WIN_HEIGHT)));
			}
			else
			{
				//right
				default_big_for_rectangle_L1(cv::Rect(0, 0, big_index_L1 + 1, WIN_HEIGHT)).copyTo(default_big_for_rectangle_fast_L1(cv::Rect(BIG_WIN_WIDTH - 1 - big_index_L1, 0, big_index_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_for_rectangle_L1(cv::Rect(big_index_L1 + 1, 0, BIG_WIN_WIDTH - (big_index_L1 + 1), WIN_HEIGHT)).copyTo(default_big_for_rectangle_fast_L1(cv::Rect(0, 0, BIG_WIN_WIDTH - (big_index_L1 + 1), WIN_HEIGHT)));
			}
			if (!L2_Full_Big_Buffer)
			{
				default_big_for_rectangle_L2(cv::Rect(0, 0, BIG_WIN_WIDTH, WIN_HEIGHT)).copyTo(default_big_for_rectangle_fast_L2(cv::Rect(0, 0, BIG_WIN_WIDTH, WIN_HEIGHT)));
			}
			else
			{
				//right
				default_big_for_rectangle_L2(cv::Rect(0, 0, big_index_L2 + 1, WIN_HEIGHT)).copyTo(default_big_for_rectangle_fast_L2(cv::Rect(BIG_WIN_WIDTH - 1 - big_index_L2, 0, big_index_L2 + 1, WIN_HEIGHT)));
				//left
				default_big_for_rectangle_L2(cv::Rect(big_index_L2 + 1, 0, BIG_WIN_WIDTH - (big_index_L2 + 1), WIN_HEIGHT)).copyTo(default_big_for_rectangle_fast_L2(cv::Rect(0, 0, BIG_WIN_WIDTH - (big_index_L2 + 1), WIN_HEIGHT)));
			}

			//logging::INFO("precommmand called-1");
			//logging::INFO("precommand called // big_index: " + to_string(big_index));

			gg += "1";
			//===========================small ha

			if (Index_Filler_L1 + 1 > WIN_WIDTH)
			{
				zintable_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(zintable_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				zeffective_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(zeffective_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				average_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(average_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				subtraction_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(subtraction_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				lowenergy_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(lowenergy_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				highenergy_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(highenergy_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				highspot_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(highspot_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));

				lowenergy_big_input_default_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(lowenergy_small_raw_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				highenergy_big_input_default_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(highenergy_small_raw_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));

			}
			else
			{

				//right
				zintable_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(zintable_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, 0, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				zintable_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(zintable_small_L1(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				//right
				zeffective_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(zeffective_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, 0, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				zeffective_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(zeffective_small_L1(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				//right
				average_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(average_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, 0, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				average_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(average_small_L1(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				//right
				subtraction_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(subtraction_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, 0, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				subtraction_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(subtraction_small_L1(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				//right
				lowenergy_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(lowenergy_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, 0, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				lowenergy_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(lowenergy_small_L1(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				//right
				highenergy_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(highenergy_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, 0, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				highenergy_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(highenergy_small_L1(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				//right
				highspot_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(highspot_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, 0, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				highspot_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(highspot_small_L1(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));



				lowenergy_big_input_default_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(lowenergy_small_raw_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, 0, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				lowenergy_big_input_default_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(lowenergy_small_raw_L1(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				//right
				highenergy_big_input_default_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(highenergy_small_raw_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, 0, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				highenergy_big_input_default_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(highenergy_small_raw_L1(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));



			}
			//logging::INFO("precommmand called-2");

			if (Index_Filler_L2 + 1 > WIN_WIDTH)
			{
				zintable_big_L2(cv::Rect(Index_Filler_L2 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(zintable_small_L2(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				zeffective_big_L2(cv::Rect(Index_Filler_L2 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(zeffective_small_L2(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				average_big_L2(cv::Rect(Index_Filler_L2 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(average_small_L2(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				subtraction_big_L2(cv::Rect(Index_Filler_L2 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(subtraction_small_L2(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				lowenergy_big_L2(cv::Rect(Index_Filler_L2 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(lowenergy_small_L2(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				highenergy_big_L2(cv::Rect(Index_Filler_L2 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(highenergy_small_L2(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				highspot_big_L2(cv::Rect(Index_Filler_L2 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(highspot_small_L2(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));


				lowenergy_big_input_default_L2(cv::Rect(Index_Filler_L2 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(lowenergy_small_raw_L2(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));
				highenergy_big_input_default_L2(cv::Rect(Index_Filler_L2 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(highenergy_small_raw_L2(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)));

			}
			else
			{

				//right
				zintable_big_L2(cv::Rect(0, 0, Index_Filler_L2 + 1, WIN_HEIGHT)).copyTo(zintable_small_L2(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L2, 0, Index_Filler_L2 + 1, WIN_HEIGHT)));
				//left
				zintable_big_L2(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L2 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)).copyTo(zintable_small_L2(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)));

				//right
				zeffective_big_L2(cv::Rect(0, 0, Index_Filler_L2 + 1, WIN_HEIGHT)).copyTo(zeffective_small_L2(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L2, 0, Index_Filler_L2 + 1, WIN_HEIGHT)));
				//left
				zeffective_big_L2(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L2 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)).copyTo(zeffective_small_L2(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)));

				//right
				average_big_L2(cv::Rect(0, 0, Index_Filler_L2 + 1, WIN_HEIGHT)).copyTo(average_small_L2(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L2, 0, Index_Filler_L2 + 1, WIN_HEIGHT)));
				//left
				average_big_L2(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L2 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)).copyTo(average_small_L2(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)));

				//right
				subtraction_big_L2(cv::Rect(0, 0, Index_Filler_L2 + 1, WIN_HEIGHT)).copyTo(subtraction_small_L2(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L2, 0, Index_Filler_L2 + 1, WIN_HEIGHT)));
				//left
				subtraction_big_L2(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L2 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)).copyTo(subtraction_small_L2(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)));

				//right
				lowenergy_big_L2(cv::Rect(0, 0, Index_Filler_L2 + 1, WIN_HEIGHT)).copyTo(lowenergy_small_L2(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L2, 0, Index_Filler_L2 + 1, WIN_HEIGHT)));
				//left
				lowenergy_big_L2(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L2 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)).copyTo(lowenergy_small_L2(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)));

				//right
				highenergy_big_L2(cv::Rect(0, 0, Index_Filler_L2 + 1, WIN_HEIGHT)).copyTo(highenergy_small_L2(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L2, 0, Index_Filler_L2 + 1, WIN_HEIGHT)));
				//left
				highenergy_big_L2(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L2 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)).copyTo(highenergy_small_L2(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)));

				//right
				highspot_big_L2(cv::Rect(0, 0, Index_Filler_L2 + 1, WIN_HEIGHT)).copyTo(highspot_small_L2(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L2, 0, Index_Filler_L2 + 1, WIN_HEIGHT)));
				//left
				highspot_big_L2(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L2 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)).copyTo(highspot_small_L2(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)));



				lowenergy_big_input_default_L2(cv::Rect(0, 0, Index_Filler_L2 + 1, WIN_HEIGHT)).copyTo(lowenergy_small_raw_L2(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L2, 0, Index_Filler_L2 + 1, WIN_HEIGHT)));
				//left
				lowenergy_big_input_default_L2(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L2 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)).copyTo(lowenergy_small_raw_L2(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)));

				//right
				highenergy_big_input_default_L2(cv::Rect(0, 0, Index_Filler_L2 + 1, WIN_HEIGHT)).copyTo(highenergy_small_raw_L2(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L2, 0, Index_Filler_L2 + 1, WIN_HEIGHT)));
				//left
				highenergy_big_input_default_L2(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L2 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)).copyTo(highenergy_small_raw_L2(cv::Rect(0, 0, WIN_WIDTH - (Index_Filler_L2 + 1), WIN_HEIGHT)));


			}
			//logging::INFO("precommmand called-3");

			gg += "2";

			Update_FB_Seeker();

			//frame_number_status_scan_online();
			//logging::INFO("precommmand called-4");

			gg += "3";
			back_up_from_view_small();
			//logging::INFO("precommmand called-5");

			gg += "4";
			fill_zeff_pp_mat();
			gg += "5";
			//logging::INFO("precommmand called-6");

			//===========================================================amade sazi vector tarikh va zaman
			//update_size_index_vec();//????
			gg += "7";
			//logging::INFO("precommmand called-7");

			//===========================================================kadrkeshi  dar zamane stop
			//draw_rect_on_stop_time();//???
			gg += "8";
			//logging::INFO("precommmand called-8");

		}
		else
		{
			//logging::INFO("precommand called // big_index_OTS: " + to_string(big_index_OTS));
			gg += "OTS//";
			gg += "1";
			gg += "2";
			//===========================small ha

			zintable_big_OTS(cv::Rect(big_index_OTS - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT_OTS)).copyTo(zintable_small_OTS(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_OTS)));
			zeffective_big_OTS(cv::Rect(big_index_OTS - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT_OTS)).copyTo(zeffective_small_OTS(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_OTS)));
			average_big_OTS(cv::Rect(big_index_OTS - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT_OTS)).copyTo(average_small_OTS(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_OTS)));
			subtraction_big_OTS(cv::Rect(big_index_OTS - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT_OTS)).copyTo(subtraction_small_OTS(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_OTS)));
			lowenergy_big_OTS(cv::Rect(big_index_OTS - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT_OTS)).copyTo(lowenergy_small_OTS(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_OTS)));
			highenergy_big_OTS(cv::Rect(big_index_OTS - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT_OTS)).copyTo(highenergy_small_OTS(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_OTS)));

			gg += "3";

			if (big_index_OTS >= WIN_WIDTH + WIN_WIDTH - 1)
			{
				number_of_frame = big_index_OTS - (WIN_WIDTH + WIN_WIDTH - 1);
			}
			else
			{
				number_of_frame = 0;
			}

			startpoint_view_index_fast = big_index_OTS - (WIN_WIDTH - 1);

			gg += "4";
			view_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_VIEW)).copyTo(view_small_semi_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_VIEW)));

			gg += "5";
			//===========================================================amade sazi vector tarikh va zaman
			size_datetime_vec = index_obj_vec_OTS.size();
			gg += "6";
		}

	}
	catch (const std::exception& e)
	{
		logging::INFO("error // precommand   " + gg);
		logging::INFO(e.what());
	}
}
void Clear_Big_Buffers_L1(bool two_sides, int p_1, int p_2)
{

	if (two_sides)
	{
		//                 p_2                                p_1
		//----------------------------------------------------------------------------
		//                  |                                  |                     |
		//   left side      |          BIG_BUFFER              |       right side    |
		//                  |                                  |                     |
		//----------------------------------------------------------------------------

		int start_right = p_1;
		int width_right = BIG_WIN_WIDTH - 1 - p_1 + 1;

		int start_left = 0;
		int width_left = p_2 + 1;


		//right
		zintable_big_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = cv::Scalar(0x0000);
		zeffective_big_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = cv::Scalar(0);
		average_big_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = cv::Scalar(0xffff);
		subtraction_big_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = cv::Scalar(0xffff);
		lowenergy_big_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = cv::Scalar(0xffff);
		highenergy_big_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = cv::Scalar(0xffff);
		highspot_big_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = cv::Scalar(0xffff);
		lowenergy_big_input_default_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = cv::Scalar(0xffff);
		highenergy_big_input_default_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = cv::Scalar(0xffff);
		default_big_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = Scalar(255, 255, 255);
		default_big_for_rectangle_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = Scalar(255, 255, 255);
		default_big_for_vari_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = Scalar(255, 255, 255);
		default_big_for_sen_L1(cv::Rect(start_right, 0, width_right, WIN_HEIGHT)) = Scalar(255, 255, 255);



		//left
		zintable_big_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = cv::Scalar(0x0000);
		zeffective_big_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = cv::Scalar(0);
		average_big_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = cv::Scalar(0xffff);
		subtraction_big_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = cv::Scalar(0xffff);
		lowenergy_big_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = cv::Scalar(0xffff);
		highenergy_big_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = cv::Scalar(0xffff);
		highspot_big_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = cv::Scalar(0xffff);
		lowenergy_big_input_default_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = cv::Scalar(0xffff);
		highenergy_big_input_default_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = cv::Scalar(0xffff);
		default_big_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = Scalar(255, 255, 255);
		default_big_for_rectangle_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = Scalar(255, 255, 255);
		default_big_for_vari_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = Scalar(255, 255, 255);
		default_big_for_sen_L1(cv::Rect(start_left, 0, width_left, WIN_HEIGHT)) = Scalar(255, 255, 255);

	}
	else
	{
		//                        p_1                p_2
		//----------------------------------------------------------------------------
		//                         |                  |                              |
		//                         |      OBJECT      |                              |
		//                         |                  |                              |       
		//----------------------------------------------------------------------------

		int start_point = p_1;
		int width_measure = p_2 - p_1 + 1;

		//logging::INFO("start_point L1 SHOULD BE DELETED: " + to_string(start_point));
		//logging::INFO("width_measure L1 SHOULD BE DELETED: " + to_string(width_measure));

		zintable_big_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = cv::Scalar(0x0000);
		zeffective_big_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = cv::Scalar(0);
		average_big_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = cv::Scalar(0xffff);
		subtraction_big_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = cv::Scalar(0xffff);
		lowenergy_big_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = cv::Scalar(0xffff);
		highenergy_big_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = cv::Scalar(0xffff);
		highspot_big_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = cv::Scalar(0xffff);
		lowenergy_big_input_default_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = cv::Scalar(0xffff);
		highenergy_big_input_default_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = cv::Scalar(0xffff);
		default_big_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = Scalar(255, 255, 255);
		default_big_for_rectangle_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = Scalar(255, 255, 255);
		default_big_for_vari_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = Scalar(255, 255, 255);
		default_big_for_sen_L1(cv::Rect(start_point, 0, width_measure, WIN_HEIGHT)) = Scalar(255, 255, 255);

	}
	L1_Fill_based_mode_direction();
}
void Clear_Small_Buffers_L1()
{
	low_raw_5_L1(cv::Rect(0, 0, 5, WIN_HEIGHT)) = cv::Scalar(0xffff);
	high_raw_5_L1(cv::Rect(0, 0, 5, WIN_HEIGHT)) = cv::Scalar(0xffff);
	zintable_3_L1(cv::Rect(0, 0, 3, WIN_HEIGHT)) = cv::Scalar(0x0000);
	average_3_L1(cv::Rect(0, 0, 3, WIN_HEIGHT)) = cv::Scalar(0xffff);
	subtraction_3_L1(cv::Rect(0, 0, 3, WIN_HEIGHT)) = cv::Scalar(0xffff);
	zeffective_3_L1(cv::Rect(0, 0, 3, WIN_HEIGHT)) = cv::Scalar(0);

	low_raw_5_sh_L1(cv::Rect(0, 0, 5, WIN_HEIGHT)) = cv::Scalar(0xffff);
	high_raw_5_sh_L1(cv::Rect(0, 0, 5, WIN_HEIGHT)) = cv::Scalar(0xffff);
	raw_sharped_line_L1(cv::Rect(0, 0, 1, WIN_HEIGHT)) = cv::Scalar(0xffff);
	tmp_colormat_L1(cv::Rect(0, 0, 1, WIN_HEIGHT)) = cv::Scalar(255, 255, 255);
}
void Grab_Line_State_Changer(bool generator_current_state)
{
	try
	{
		if (generator_current_state)
		{
			should_grab_line = true;
		}
		else
		{
			should_grab_line = false;
		}

		if (should_grab_line_previous != should_grab_line)
		{
			logging::INFO("Grab_Line_State_Changer Called >>>  should_grab_line: " + to_string(should_grab_line));
		}

		should_grab_line_previous = should_grab_line;
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // Grab_Line_State_Changer");
		logging::INFO(e.what());
	}
}
void L1_Insert_Lines()
{
	try
	{

		for (int i = 0; i < WIN_HEIGHT; i++)
		{
			//low_raw_5_L1.at<unsigned short>(i, 4) = input_buffer_L1.at<unsigned short>(Point(process_index_buffer_copy_L1, i));
			//high_raw_5_L1.at<unsigned short>(i, 4) = input_buffer_L1.at<unsigned short>(Point(process_index_buffer_copy_L1, i + WIN_HEIGHT));

			/*low_raw_5_L1.at<unsigned short>(i, 4) = gs_line_input_org_L1[i];
			high_raw_5_L1.at<unsigned short>(i, 4) = gs_line_input_org_L1[i + WIN_HEIGHT];*/
#ifdef _XRAY_100100D_EXPORTS
			low_raw_5_L1.at<unsigned short>(i, 4) = HLP_2_L1.at<unsigned short>(i, 0);
			high_raw_5_L1.at<unsigned short>(i, 4) = HLP_2_L1.at<unsigned short>(i + WIN_HEIGHT, 0);
#endif

#ifdef _XRAY_7555D_EXPORTS
			low_raw_5_L1.at<unsigned short>(i, 4) = HLP_1_L1.at<unsigned short>(i, 0);
			high_raw_5_L1.at<unsigned short>(i, 4) = HLP_1_L1.at<unsigned short>(i + WIN_HEIGHT, 0);
#endif

#ifdef _XRAY_6040D_EXPORTS
			low_raw_5_L1.at<unsigned short>(i, 4) = HLP_1_L1.at<unsigned short>(i, 0);
			high_raw_5_L1.at<unsigned short>(i, 4) = HLP_1_L1.at<unsigned short>(i + WIN_HEIGHT, 0);
#endif

		}
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 insert lines");
		logging::INFO(e.what());
	}
}
void L1_Filter()
{
	try
	{
		SharpFilter_L1(low_raw_5_L1, 2, 1, 5, 7, 1, low_raw_5_sh_L1);
		SharpFilter_L1(high_raw_5_L1, 2, 10, 7, 3, 1, high_raw_5_sh_L1);
		//raw_line_L1 = low_raw_5_sh_L2.colRange(2, 3);
		raw_sharped_line_L1 = 0.75 * low_raw_5_sh_L1.colRange(2, 3) + 0.25 * high_raw_5_sh_L1.colRange(2, 3);


		//SharpFilter_L1(low_raw_5_L1, 1, 1, 5, 7, 1.0, low_raw_5_sh_L1);
		//SharpFilter_L1(high_raw_5_L1, 1, 3, 3, 3, 0.8, high_raw_5_sh_L1);
		//raw_sharped_line_L1 = 0.9 * low_raw_5_L1.colRange(2, 3) + 0.1 * high_raw_5_L1.colRange(2, 3);
		//SharpFilter(low_raw_5, 0.5, 1, 10, 7, 1, low_raw_5_sh);
		//SharpFilter(high_raw_5, 2, 10, 11, 3, 1, high_raw_5_sh);
		//raw_sharped_line = 0.5 * low_raw_5_sh.colRange(2, 3) + 0.5 * high_raw_5_sh.colRange(2, 3);
		//SharpFilter(low_raw_5, 1.5, 1, 10, 7, 1, low_raw_5_sh);
		//SharpFilter(high_raw_5, 2, 10, 11, 3, 1, high_raw_5_sh);
		//raw_sharped_line = 0.65 * low_raw_5_sh.colRange(2, 3) + 0.35 * high_raw_5_sh.colRange(2, 3);
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 filter");
		logging::INFO(e.what());
	}
}
void L1_Fill_3c_Mats()
{
	try
	{
		for (int k = 0; k < WIN_HEIGHT; k++)
		{
			ZCompute_L1(k);
			zintable_3_L1.at<unsigned short>(k, 2) = z_in_table_L1;
			zeffective_3_L1.at<float>(k, 2) = Z_EFFECTIVE_L1;
			average_3_L1.at<unsigned short>(k, 2) = AVERAGE_L1;
			subtraction_3_L1.at<unsigned short>(k, 2) = SUBB_L1;
		}
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 fill 3c mats");
		logging::INFO(e.what());
	}
}
void L1_CL_Line_Generator()
{
	try
	{
		for (int k = offset_tunnel_up_L1; k < offset_tunnel_down_L1; k++)
		{
			coory_L1 = static_cast<int>(zintable_3_L1.at<unsigned short>(k, 0));
			coorx_L1 = 743 - (raw_sharped_line_L1.at<unsigned short>(k, 0) * scale);
			if (coorx_L1 < 0)
				coorx_L1 = 0;
			if (coorx_L1 >= 60 && coorx_L1 <= 712)
			{
				hamgen_L1 += 1;
			}
			MonoColorLine_L1 += coorx_L1;
			MonoAtomiLine_L1 += coory_L1;
			intensity_L1 = pallet.at<Vec3b>(coory_L1, coorx_L1);

			//if (zeffective_3.at<float>(k, 0) == 7.0)
			//{
			//	pallet_RGBX.at<Vec3b>(Point(coorx, coory)) = BLUE_I;
			//}
			//else if (zeffective_3.at<float>(k, 0) == 8.0 )
			//{
			//	pallet_RGBX.at<Vec3b>(Point(coorx, coory)) = GREEN_I;
			//}
			//else if (zeffective_3.at<float>(k, 0) == 9.0 )
			//{
			//	pallet_RGBX.at<Vec3b>(Point(coorx, coory)) = RED_I;
			//}

			if ((average_3_L1.at<unsigned short>(k, 0)) > thr_bakground_L1 || ((subtraction_3_L1.at<unsigned short>(k, 0)) < 0 && (average_3_L1.at<unsigned short>(k, 0)) > thr_bakground_L1 - 1000))
			{
				//white pixel
				num_of_white_pixel_per_line_L1 += 1;
				intensity_L1[0] = 255;
				intensity_L1[1] = 255;
				intensity_L1[2] = 255;

				Max_Neighbor_CNT_Object_Pixles_L1 = 0;
			}
			else
			{
				num_of_object_pixel_per_line_L1 += 1;
				Max_Neighbor_CNT_Object_Pixles_L1 += 1;

				if (Max_Neighbor_CNT_Object_Pixles_L1 > Max_Neighbor_Object_Pixles_L1)
				{
					Max_Neighbor_Object_Pixles_L1 = Max_Neighbor_CNT_Object_Pixles_L1;
				}
			}

			//if (average_3_L1.at<unsigned short>(k, 0) < unsigned short(1500))
			//{
			//	//white pixel
			//	double ave_gray_L1 = 0.066 * (average_3_L1.at<unsigned short>(k, 0));
			//	intensity_L1[0] = int(ave_gray_L1);
			//	intensity_L1[1] = int(ave_gray_L1);
			//	intensity_L1[2] = int(ave_gray_L1);
			//}

			tmp_colormat_L1.at<Vec3b>(Point(0, k)) = intensity_L1;

			//average kole line
			tmp_colorline_L1[k] = (intensity_L1[0] + intensity_L1[1] + intensity_L1[2]) / 3;
			average_of_line_L1 = average_of_line_L1 + tmp_colorline_L1[k];
		}
		average_of_line_L1 = average_of_line_L1 / pixel_perline_process_L1;

		//logging::INFO("L1 >.> num_of_object_pixel_per_line_L1: " + to_string(num_of_object_pixel_per_line_L1)
		//	+ "  num_of_white_pixel_per_line_L1:" + to_string(num_of_white_pixel_per_line_L1));

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//white line or object line?
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 cl line gen");
		logging::INFO(e.what());
	}
}
void L1_White_Object_Line_Detector()
{
	try
	{
		//logging::INFO("L1 >.> >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

		double percent_hamgen = (static_cast<double>(hamgen_L1) / static_cast<double>(pixel_perline_process_L1)) * 100.0;
		Average_MonoColorLine_L1 = double(MonoColorLine_L1) / double(pixel_perline_process_L1);
		double Average_MonoAtomiLine_L1 = double(MonoAtomiLine_L1) / double(pixel_perline_process_L1);

		//percent_line_is_white_L1 = (static_cast<double>(num_of_white_pixel_per_line_L1) / static_cast<double>(pixel_perline_process_L1)) * 100.0;

		if (num_of_object_pixel_per_line_L1 < L1_atleast_object_pixles)// || Max_Neighbor_Object_Pixles_L1 < 5)//(percent_line_is_white_L1 >= L1_max_sum)
		{
			//logging::INFO("L1 >.> White Line");
			is_white_line_L1 = true;
			is_object_line_L1 = false;
			L1_Line_Status = 0;
		}
		else
		{
			//logging::INFO("L1 >.> NO White Line");
			//-----------------------------------
			//  moshakhas kardane noe line
			//-----------------------------------
			for (int k = offset_tunnel_up_L1; k < offset_tunnel_down_L1; k++)
			{
				if (tmp_colorline_L1[k] <= black_line_thr)
					percent_black_line_L1 += 1;
				else if (tmp_colorline_L1[k] > black_line_thr && tmp_colorline_L1[k] <= dark_line_thr)
					percent_dark_line_L1 += 1;
				else if (tmp_colorline_L1[k] > dark_line_thr && tmp_colorline_L1[k] <= mid_line_thr)
					percent_mid_line_L1 += 1;
				else if (tmp_colorline_L1[k] > mid_line_thr && tmp_colorline_L1[k] <= light_line_thr)
					percent_light_line_L1 += 1;
				else if (tmp_colorline_L1[k] > light_line_thr)
					percent_white_line_L1 += 1;
			}
			percent_black_line_L1 = (static_cast<double>(percent_black_line_L1) / static_cast<double>(pixel_perline_process_L1)) * 100.0;
			percent_dark_line_L1 = (static_cast<double>(percent_dark_line_L1) / static_cast<double>(pixel_perline_process_L1)) * 100.0;
			percent_mid_line_L1 = (static_cast<double>(percent_mid_line_L1) / static_cast<double>(pixel_perline_process_L1)) * 100.0;
			percent_light_line_L1 = (static_cast<double>(percent_light_line_L1) / static_cast<double>(pixel_perline_process_L1)) * 100.0;
			percent_white_line_L1 = (static_cast<double>(percent_white_line_L1) / static_cast<double>(pixel_perline_process_L1)) * 100.0;

			is_object_line_L1 = true;
			is_white_line_L1 = false;
			L1_Line_Status = 1;

			//logging::INFO("L1 >.> percent_line_black: " + to_string(percent_black_line_L1));
			//logging::INFO("L1 >.> percent_line_dark: " + to_string(percent_dark_line_L1));
			//logging::INFO("L1 >.> percent_line_mid: " + to_string(percent_mid_line_L1));
			//logging::INFO("L1 >.> percent_line_light: " + to_string(percent_light_line_L1));
			//logging::INFO("L1 >.> percent_line_white: " + to_string(percent_white_line_L1));

			if (percent_black_line_L1 >= 99.0)
			{
				L1_dark_lines_coming += 1;
				if (L1_dark_lines_coming == L1_check_number_dark_lines)
				{
					L1_flag_see_feature_1 = true;
					L1_x_gen_is_off = true;

					//logging::INFO("L1 >.> L1_flag_see_feature_1: 1      L1_x_gen_is_off: 1 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
					//cout << "L1_flag_see_feature_1: " << 1 << "  L1_x_gen_is_off: " << 1 << endl;
				}
				is_object_line_L1 = false;
				is_white_line_L1 = false;
				L1_Line_Status = -1;
			}
			if (L1_flag_see_feature_1)
			{
				if ((percent_black_line_L1 + percent_dark_line_L1) >= 99.0 && percent_black_line_L1 <= 70.0)
				{
					L1_number_blue_line_coming += 1;
					if (L1_number_blue_line_coming == 3)
					{
						L1_flag_see_feature_2 = true;
						//logging::INFO("L1 >.> L1_flag_see_feature_2:   1 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
						//cout << "L1_flag_see_feature_2: " << 1 << endl;
						L1_number_blue_line_coming = 0;
					}
				}
			}
			if (L1_flag_see_feature_2)
			{
				L1_dark_lines_coming = 0;
				L1_flag_see_feature_1 = false;
				L1_flag_see_feature_2 = false;
				L1_flag_to_ignore_lines = true;
				//logging::INFO("L1 >.> L1_flag_to_ignore_lines:   1 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
				//cout << "L1_flag_to_ignore_lines: " << 1 << endl;
			}
			if (L1_flag_to_ignore_lines)
			{
				L1_ignore_lines_coming += 1;
				if (L1_ignore_lines_coming == remove_starting_gen_number)
				{
					L1_flag_to_ignore_lines = false;
					L1_x_gen_is_off = false;
					L1_ignore_lines_coming = 0;
					//logging::INFO("L1 >.> L1_x_gen_is_off:   0 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
					//cout << "L1_x_gen_is_off: " << 0 << endl;
				}
				is_object_line_L1 = false;
				is_white_line_L1 = false;
				L1_Line_Status = -1;

			}
			if (L1_x_gen_is_off)
			{
				is_object_line_L1 = false;
				is_white_line_L1 = false;
				L1_Line_Status = -1;
				//logging::INFO("L1 >.> L1_x_gen_is_off_1000");
			}

			if ((percent_black_line_L1 + percent_dark_line_L1) >= 99.9)
			{
				is_object_line_L1 = false;
				is_white_line_L1 = false;
				L1_Line_Status = -1;

				//logging::INFO("L1 >.> L1_x_gen_is_off_1001");
			}

			if ((percent_black_line_L1 + percent_dark_line_L1 + percent_mid_line_L1) >= 99.9)
			{
				is_object_line_L1 = false;
				is_white_line_L1 = false;
				L1_Line_Status = -1;

				//logging::INFO("L1 >.> L1_x_gen_is_off_1002");
			}

			if (percent_black_line_L1 >= 90.0 || percent_dark_line_L1 >= 90.0 || percent_mid_line_L1 >= 90.0 || percent_light_line_L1 >= 90.0)
			{
				is_object_line_L1 = false;
				is_white_line_L1 = false;
				L1_Line_Status = -1;

				//logging::INFO("L1 >.> L1_x_gen_is_off_1003");
				//cout << "RULE1 " << endl;
			}

			if (percent_hamgen >= double(remove_stoping_gen_number))
			{
				is_object_line_L1 = false;
				is_white_line_L1 = false;
				L1_Line_Status = -1;
				//logging::INFO("L1 >.> L1_x_gen_is_off_1004");
			}

			if (num_of_object_pixel_per_line_L1 > 800 && Average_MonoColorLine_L1 < 160.0 && Average_MonoAtomiLine_L1 < 480.0)
			{
				is_object_line_L1 = false;
				is_white_line_L1 = false;
				//logging::INFO("L1 >.> L1_x_gen_is_off_1005");
			}

			if (Average_MonoColorLine_L1 < 5.0)
			{
				is_object_line_L1 = false;
				is_white_line_L1 = true;
				//logging::INFO("L1 >.> L1_x_gen_is_off_1006");
			}

		}

		//if (Max_Neighbor_Object_Pixles_L2 >= int(0.98 * pixel_perline_process_L2) && !is_white_line_L1)
		//{
		//	is_white_line_L1 = false;
		//	is_object_line_L1 = false;
		//}

		if (is_object_line_L1)
		{
			All_Object_Lines_L1 += 1;
		}

		//logging::INFO("All_Object_Lines_L1: " + to_string(All_Object_Lines_L1));


		logging::INFO("L1 >.> Line status || W:" + to_string(is_white_line_L1) + " * O:" + to_string(is_object_line_L1));


		/*logging::INFO("L1 >> Line status || W:" + to_string(is_white_line_L1)
			+ " * O:" + to_string(is_object_line_L1)
			+ " * O_P:" + to_string(num_of_object_pixel_per_line_L1)
			+ " * P_H:" + to_string(percent_hamgen)
			+ " * N_B_O_L:" + to_string(Object_Lines_from_Bgn_Obj_L1)
			+ " * L1_N_B_P:" + to_string(L1_Num_of_Bgn_Points) 
			+ " * L2_N_B_P:" + to_string(L2_Num_of_Bgn_Points)
			+ " * N_O:" + to_string(NEW_Obj_is_Coming_L1)
			+ " * L1_S_E_W:" + to_string(L1_Has_Seen_Enough_Width_as_reference)
			+ " * A_W_M:" + to_string(Allowed_White_Line_Mode_L1)
			+ " * A_M_C:" + to_string(Average_MonoColorLine_L1)
			+ " * A_M_A:" + to_string(Average_MonoAtomiLine_L1));*/

		//logging::INFO("L1 >> Line status || WHITE: " + to_string(is_white_line_L1)
		//	+ " OBJECT: " + to_string(is_object_line_L1)
		//	+ " OBJECT_PIXELS: " + to_string(num_of_object_pixel_per_line_L1)
		//	+ " percent_hamgen: " + to_string(percent_hamgen)
		//	+ " Average_MonoColorLine_L1: " + to_string(Average_MonoColorLine_L1)
		//	+ " Average_MonoAtomiLine_L1: " + to_string(Average_MonoAtomiLine_L1)
		//	+ " Object_Lines_from_Bgn_Obj_L1: " + to_string(Object_Lines_from_Bgn_Obj_L1)
		//	+ " L1_Num_of_Bgn_Points: " + to_string(L1_Num_of_Bgn_Points)
		//	+ " L2_Num_of_Bgn_Points: " + to_string(L2_Num_of_Bgn_Points)
		//	+ " NEW_Obj_is_Coming_L1: " + to_string(NEW_Obj_is_Coming_L1)
		//	+ " Object_Lines_from_Bgn_Obj_L1: " + to_string(Object_Lines_from_Bgn_Obj_L1)
		//	+ " L1_Has_Seen_Enough_Width_as_reference: " + to_string(L1_Has_Seen_Enough_Width_as_reference)
		//	+ " Allowed_White_Line_Mode_L1: " + to_string(Allowed_White_Line_Mode_L1));

		//cout << "WHITE: " << is_white_line_L1 << " OBJECT: " << is_object_line_L1 << endl;
		//logging::INFO("L1 >> Line status || BIG_INDEX_L1: " + to_string(big_index_L1) + " INDEX_FILL_L1: " + to_string(Index_Filler_L1));
		//is_object_line_L1 = true;
		//is_white_line_L1 = false;
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 white obj line detector");
		logging::INFO(e.what());
	}
}
void L1_Check_Gen_Status()
{
	try
	{
		//logging::INFO("L1_num_white_line_in_gen_off: " + to_string(L1_num_white_line_in_gen_off));
		if (L1_x_gen_is_off)
		{
			if (is_white_line_L1)
			{
				L1_num_white_line_in_gen_off += 1;
			}
			if (L1_num_white_line_in_gen_off == 10)
			{
				

				L1_num_white_line_in_gen_off = 0;
				L1_x_gen_is_off = false;
				L1_flag_see_feature_1 = false;
				L1_dark_lines_coming = 0;
				L1_number_blue_line_coming = 0;
				L1_flag_see_feature_2 = false;
				L1_flag_to_ignore_lines = false;
				L1_ignore_lines_coming = 0;
				//cout << "permission gen " << endl;
			}
		}

		if (!L1_generator_log_status_on && !L1_x_gen_is_off)
		{
			//logging::INFO("L1 >> Genrator ON");
			L1_generator_log_status_on = true;
			L1_generator_log_status_off = false;
		}

		if (!L1_generator_log_status_off && L1_x_gen_is_off)
		{
			//logging::INFO("L1 >> Genrator OFF");
			L1_generator_log_status_off = true;
			L1_generator_log_status_on = false;
		}


	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 check gen status");
		logging::INFO(e.what());
	}
}
int Compute_Current_Width_Instance(int start, int stop)
{
	int current_width = 0;
	if (stop < start)
	{
		current_width = (stop + 1) + (BIG_WIN_WIDTH - 1 - start + 1);
	}
	else
	{
		current_width = stop - start + 1;
	}
	return current_width;
}
void L1_Set_Begining_Object_Value()
{
	if ((big_index_L1 + 1) - object_width_thereshold_L1 >= 0)
	{
		Begining_Object_L1 = (big_index_L1 + 1) - object_width_thereshold_L1;
	}
	else
	{
		Begining_Object_L1 = BIG_WIN_WIDTH + big_index_L1 + 1 - object_width_thereshold_L1;
	}

	L1_Num_of_Bgn_Points += 1;

	UPDATED_BEGINING_VALID_POINT_IN_L1_VEC[Seek_on_UPDATED_BEGINING_VALID_POINT_IN_L1_VEC] = Begining_Object_L1;

	logging::INFO("L1 >> * BEGN >> Last_index: " + to_string(Seek_on_UPDATED_BEGINING_VALID_POINT_IN_L1_VEC) 
		+ " UPDATED_BEGINING_VALID_POINT_IN_L1_VEC: " + to_string(UPDATED_BEGINING_VALID_POINT_IN_L1_VEC[Seek_on_UPDATED_BEGINING_VALID_POINT_IN_L1_VEC])
		+ " IF_L1: " + to_string(Index_Filler_L1)
		+ " bx_L1: " + to_string(big_index_L1)
		+ " IF_L2: " + to_string(Index_Filler_L2)
		+ " bx_L2: " + to_string(big_index_L2));
	Seek_on_UPDATED_BEGINING_VALID_POINT_IN_L1_VEC += 1;

	begin_L1 = true;
}

void L1_Set_Ending_Object_Value()
{
	Ending_Object_L1 = big_index_L1;
	end_L1 = true;
}
void L1_Write_Object_Information_BUFFER()
{
	// baraye moshakhas kardan parametrhaye object bayad 
	//arz jesm ra moshakhas konim ke 2 halat etefagh mioftad

	int current_object_width_instance = 0;

	if (Ending_Object_L1 < Begining_Object_L1)//halati ke object nesf nesf ast dar ebteda va entehaye buffer
	{
		current_object_width_instance = (Ending_Object_L1 + 1) + (BIG_WIN_WIDTH - 1 - Begining_Object_L1 + 1);
	}
	else
	{
		current_object_width_instance = Ending_Object_L1 - Begining_Object_L1 + 1;
	}

	Instance_Obj_L1.start_point_object = Begining_Object_L1;
	Instance_Obj_L1.stop_point_object = Ending_Object_L1;
	Instance_Obj_L1.Width_Obj = current_object_width_instance;
	Instance_Obj_L1.Time_Scan = Get_Time();

	UPDATED_WIDTH_FROM_L1_VEC[Seek_on_UPDATED_WIDTH_FROM_L1_VEC] = Instance_Obj_L1.Width_Obj;
	Seek_on_UPDATED_WIDTH_FROM_L1_VEC += 1;

	condVar_L1.notify_one();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ flag for 1 order after object compeleted to flag_x_gen_can_be_off to equal 1

	//New_Obj_from_L1 = true;
	L1_flag_permit_to_x_gen_off = true;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ save object to vector for forward/backward

	thread List_Obj_Filler_thread_L1 = thread(L1_List_Obj_Filler);
	List_Obj_Filler_thread_L1.detach();

	One_Obj_Passed_L1 = true;

	logging::INFO("L1 >> * ENDING(BUFFER) >>  BO_L1: " + to_string(Begining_Object_L1)
		+ " EO_L1: " + to_string(Ending_Object_L1)
		+ " bx_L1: " + to_string(big_index_L1)
		+ " IF_L1: " + to_string(Index_Filler_L1) 
		+ " WO_L1: " + to_string(Instance_Obj_L1.Width_Obj)
		+ " AllObj_L1: " + to_string(All_Object_Lines_L1)
		+ " AllObj_L2: " + to_string(All_Object_Lines_L2));
	logging::INFO("L1 >> ======================================================");
}
void L1_Write_Object_Information_REAL()
{
	// baraye moshakhas kardan parametrhaye object 
	int current_object_width_instance = 0;

	if (Ending_Object_L1 < Begining_Object_L1)//halati ke object nesf nesf ast 
	{
		current_object_width_instance = (Ending_Object_L1 + 1) + (BIG_WIN_WIDTH - 1 - Begining_Object_L1 + 1);
	}
	else              //halati ke object kamel ast
	{
		current_object_width_instance = Ending_Object_L1 - Begining_Object_L1 + 1;
	}
	Instance_Obj_L1.start_point_object = Begining_Object_L1;
	Instance_Obj_L1.stop_point_object = Ending_Object_L1;
	Instance_Obj_L1.Width_Obj = current_object_width_instance;
	Instance_Obj_L1.Time_Scan = Get_Time();

	condVar_L1.notify_one();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~flag for 1 order after object compeleted to flag_x_gen_can_be_off to equal 1
	//New_Obj_from_L1 = true;
	L1_flag_permit_to_x_gen_off = true;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ object counter
	OBJCNT_OUT += 1;
	OBJCNT_OUT_AFTER_GENERATOR_ON += 1;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ save object to vector for forward/backward

	thread List_Obj_Filler_thread_L1 = thread(L1_List_Obj_Filler);
	List_Obj_Filler_thread_L1.detach();

	One_Obj_Passed_L1 = true;

	logging::INFO("L1 >> * ENDING(REAL) >>  BO_L1: " + to_string(Begining_Object_L1)
		+ " EO_L1: " + to_string(Ending_Object_L1)
		+ " bx_L1: " + to_string(big_index_L1)
		+ " IF_L1: " + to_string(Index_Filler_L1)
		+ " WO_L1: " + to_string(Instance_Obj_L1.Width_Obj)
		+ " AllObj_L1: " + to_string(All_Object_Lines_L1)
		+ " AllObj_L2: " + to_string(All_Object_Lines_L2));
	logging::INFO("L1 >> ======================================================");
}
void L1_Share_White_Block_Proccessing()
{
	if (OBJCNT_OUT_AFTER_GENERATOR_ON >= 1)
	{
		ignore_first_object_after_generator_turn_on = false;
	}

	//////////////////////////////////////////
	//    tedad khathaye sefid rad shode agar 
	//    bishtar az 30 bashad va object niz 
	//    be payan reside ast generator 
	//    mitavanad khamoosh shavd                                                                               
	//////////////////////////////////////////

	if (White_Lines_after_End_Obj_L1 >= 30 && L1_flag_permit_to_x_gen_off && L1_Num_of_Bgn_Points == L2_Num_of_Bgn_Points)
	{
		L1_flag_permit_to_x_gen_off = false;
		L1_x_gen_could_be_off = 1;
		logging::INFO("L1 >> L1_x_gen_could_be_off : " + to_string(L1_x_gen_could_be_off));
	}

	//////////////////////////////////////////////
	// let pass white line until white line offset
	//////////////////////////////////////////////

	if (White_Lines_after_End_Obj_L1 < Allowed_White_Lines_after_Object)
	{
		Allowed_White_Line_Mode_L1 = true;
	}
	else
	{
		Allowed_White_Line_Mode_L1 = false;
	}

	if (Allowed_White_Line_Mode_L1 != previous_status_Allowed_White_Line_Mode_L1)
	{
		//logging::INFO("L1 >> Allowed_White_Line_Mode : " + to_string(Allowed_White_Line_Mode_L1));
	}

	previous_status_Allowed_White_Line_Mode_L1 = Allowed_White_Line_Mode_L1;
}
void L1_Process_White_Object_Line()
{
	try
	{
		//***********************
		count_line_index_L1 += 1; // for blink
		//***********************
		//logging::INFO("big_index_L1: " + to_string(big_index_L1));
		big_index_L1 += 1;
		big_index_all_L1 += 1;
		if (big_index_L1 == BIG_WIN_WIDTH)
		{
			big_index_L1 = 0;
			L1_Full_Big_Buffer = true;
			L1_Full_Big_Buffer_Count += 1;
		}
		//***********************

		//for (int k = 0; k < WIN_HEIGHT; k++)
		//{
		//	zintable_big_L1.at<unsigned short>(k, big_index_L1) = zintable_3_L1.at<unsigned short>(k, 0);
		//	if (k < offset_tunnel_up_L1 || k > offset_tunnel_down_L1 || (average_3_L1.at<unsigned short>(k, 0)) > thr_bakground || ((subtraction_3_L1.at<unsigned short>(k, 0)) < 0 && (average_3_L1.at<unsigned short>(k, 0)) > thr_bakground - 1000))
		//	{
		//		zeffective_big_L1.at<float>(k, big_index_L1) = 0.0f;
		//	}
		//	else
		//	{
		//		zeffective_big_L1.at<float>(k, big_index_L1) = zeffective_3_L1.at<float>(k, 0);
		//	}
		//	average_big_L1.at<unsigned short>(k, big_index_L1) = average_3_L1.at<unsigned short>(k, 0);
		//	subtraction_big_L1.at<unsigned short>(k, big_index_L1) = subtraction_3_L1.at<unsigned short>(k, 0);
		//	lowenergy_big_L1.at<unsigned short>(k, big_index_L1) = low_raw_5_sh_L1.at<unsigned short>(k, 2);
		//	highenergy_big_L1.at<unsigned short>(k, big_index_L1) = high_raw_5_sh_L1.at<unsigned short>(k, 2);
		//	lowenergy_big_input_default_L1.at<unsigned short>(k, big_index_L1) = low_raw_5_L1.at<unsigned short>(k, 2);
		//	highenergy_big_input_default_L1.at<unsigned short>(k, big_index_L1) = high_raw_5_L1.at<unsigned short>(k, 2);
		//	//highspot compution
		//	coory_L1 = static_cast<int>(zintable_3_L1.at<unsigned short>(k, 0));
		//	coorx_L1 = 743 - static_cast<int>(((low_raw_5_sh_L1.at<unsigned short>(k, 2) + high_raw_5_sh_L1.at<unsigned short>(k, 2)) / 2) * scale);
		//	if (coorx_L1 < 0)
		//		coorx_L1 = 0;
		//	intensity_L1 = pallet_high.at<Vec3b>(coory_L1, coorx_L1);
		//	highspot_big_L1.at<Vec3b>(Point(big_index_L1, k)) = intensity_L1;
		//}

		if (is_object_line_L1)
		{
			//logging::INFO("OBJECT_L1");
			for (int k = 0; k < WIN_HEIGHT; k++)
			{
				zintable_big_L1.at<unsigned short>(k, big_index_L1) = zintable_3_L1.at<unsigned short>(k, 0);

				if (k < offset_tunnel_up_L1 || k > offset_tunnel_down_L1 || (average_3_L1.at<unsigned short>(k, 0)) > thr_bakground_L1 || ((subtraction_3_L1.at<unsigned short>(k, 0)) < 0 && (average_3_L1.at<unsigned short>(k, 0)) > thr_bakground_L1 - 1000))
				{
					zeffective_big_L1.at<float>(k, big_index_L1) = 0.0f;
				}
				else
				{
					zeffective_big_L1.at<float>(k, big_index_L1) = zeffective_3_L1.at<float>(k, 0);
				}

				average_big_L1.at<unsigned short>(k, big_index_L1) = average_3_L1.at<unsigned short>(k, 0);
				subtraction_big_L1.at<unsigned short>(k, big_index_L1) = subtraction_3_L1.at<unsigned short>(k, 0);
				lowenergy_big_L1.at<unsigned short>(k, big_index_L1) = low_raw_5_sh_L1.at<unsigned short>(k, 2);
				highenergy_big_L1.at<unsigned short>(k, big_index_L1) = high_raw_5_sh_L1.at<unsigned short>(k, 2);
				lowenergy_big_input_default_L1.at<unsigned short>(k, big_index_L1) = low_raw_5_L1.at<unsigned short>(k, 2);
				highenergy_big_input_default_L1.at<unsigned short>(k, big_index_L1) = high_raw_5_L1.at<unsigned short>(k, 2);

				/////////////////// highspot compution
				coory_L1 = static_cast<int>(zintable_3_L1.at<unsigned short>(k, 0));
				coorx_L1 = 743 - static_cast<int>(((low_raw_5_sh_L1.at<unsigned short>(k, 2) + high_raw_5_sh_L1.at<unsigned short>(k, 2)) / 2) * scale);
				if (coorx_L1 < 0)
					coorx_L1 = 0;
				intensity_L1 = pallet_high.at<Vec3b>(coory_L1, coorx_L1);

				highspot_big_L1.at<Vec3b>(Point(big_index_L1, k)) = intensity_L1;
			}
		}
		if (is_white_line_L1)
		{
			//logging::INFO("WHITE_L1");
			for (int k = 0; k < WIN_HEIGHT; k++)
			{
				zintable_big_L1.at<unsigned short>(k, big_index_L1) = 0.0;
				zeffective_big_L1.at<float>(k, big_index_L1) = 0.0f;
				average_big_L1.at<unsigned short>(k, big_index_L1) = 52000.0;
				subtraction_big_L1.at<unsigned short>(k, big_index_L1) = 52000.0;
				lowenergy_big_L1.at<unsigned short>(k, big_index_L1) = 52000.0;
				highenergy_big_L1.at<unsigned short>(k, big_index_L1) = 52000.0;
				lowenergy_big_input_default_L1.at<unsigned short>(k, big_index_L1) = 52000.0;
				highenergy_big_input_default_L1.at<unsigned short>(k, big_index_L1) = 52000.0;

				/////////////////// highspot compution
				//coory_L1 = 0;
				//coorx_L1 = 0;
				//intensity_L1 = pallet_high.at<Vec3b>(coory_L1, coorx_L1);

				highspot_big_L1.at<Vec3b>(Point(big_index_L1, k)) = WHITE_I;
			}

			tmp_colormat_L1(cv::Rect(0, 0, 1, WIN_HEIGHT)) = Scalar(255, 255, 255);
		}
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 process object white line");
		logging::INFO(e.what());
	}
}
void L1_Fill_Big_Mats()
{
	try
	{
		//if (begin_L1)
		//{
		//	//tmp_colormat_L1(cv::Rect(0, 60, 1, WIN_HEIGHT - 110)) = Scalar(255, 0, 0);
		//	RED_MAT_LINE(cv::Rect(0, 0, 1, WIN_HEIGHT - 300)).copyTo(default_big_L1(cv::Rect(Begining_Object_L1, 0, 1, WIN_HEIGHT - 300)));
		//	begin_L1 = false;
		//}

		//***********************
		tmp_colormat_L1(cv::Rect(0, 0, 1, WIN_HEIGHT)).copyTo(default_big_L1(cv::Rect(big_index_L1, 0, 1, WIN_HEIGHT)));
		tmp_colormat_L1(cv::Rect(0, 0, 1, WIN_HEIGHT)).copyTo(default_big_for_rectangle_L1(cv::Rect(big_index_L1, 0, 1, WIN_HEIGHT)));

		//if (end_L1)
		//{
		//	BLUE_MAT_LINE(cv::Rect(0, 0, 1, WIN_HEIGHT)).copyTo(default_big_L1(cv::Rect(big_index_L1, 0, 1, WIN_HEIGHT)));
		//	end_L1 = false;
		//}



		if (is_white_line_L1)
		{
			tmp_colormat_L1(cv::Rect(0, 0, 1, WIN_HEIGHT)).copyTo(default_big_for_vari_L1(cv::Rect(big_index_L1, 0, 1, WIN_HEIGHT)));
			tmp_colormat_L1(cv::Rect(0, 0, 1, WIN_HEIGHT)).copyTo(default_big_for_sen_L1(cv::Rect(big_index_L1, 0, 1, WIN_HEIGHT)));
		}
		else
		{
			if (M_VARI)
			{
				for (int k = 0; k < WIN_HEIGHT; k++)
				{
					coory_L1 = static_cast<int>(zintable_3_L1.at<unsigned short>(k, 0));
					coorx_L1 = 743 - (raw_sharped_line_L1.at<unsigned short>(k, 0) * scale);
					coorx_L1 = vari_editor(coorx_L1);
					if (coorx_L1 < 0)
						coorx_L1 = 0;
					if (coorx_L1 > 743)
						coorx_L1 = 743;
					intensity_L1 = pallet.at<Vec3b>(coory_L1, coorx_L1);

					if (k < offset_tunnel_up_L1 || k > offset_tunnel_down_L1 || (average_3_L1.at<unsigned short>(k, 0)) > thr_bakground_L1 || ((subtraction_3_L1.at<unsigned short>(k, 0)) < 0 && (average_3_L1.at<unsigned short>(k, 0)) > thr_bakground_L1 - 1000))
					{
						intensity_L1[0] = 255;
						intensity_L1[1] = 255;
						intensity_L1[2] = 255;
					}
					tmp_colormat_L1.at<Vec3b>(Point(0, k)) = intensity_L1;
				}
			}
			tmp_colormat_L1(cv::Rect(0, 0, 1, WIN_HEIGHT)).copyTo(default_big_for_vari_L1(cv::Rect(big_index_L1, 0, 1, WIN_HEIGHT)));

			if (M_SEN)
			{
				for (int k = 0; k < WIN_HEIGHT; k++)
				{
					coory_L1 = static_cast<int>(zintable_3_L1.at<unsigned short>(k, 0));
					coorx_L1 = 743 - (raw_sharped_line_L1.at<unsigned short>(k, 0) * scale);
					coorx_L1 = sen_peri(coorx_L1, coory_L1);
					if (coorx_L1 < 0)
						coorx_L1 = 0;
					if (coorx_L1 > 743)
						coorx_L1 = 743;
					intensity_L1 = pallet.at<Vec3b>(coory_L1, coorx_L1);

					if (k < offset_tunnel_up_L1 || k > offset_tunnel_down_L1 || (average_3_L1.at<unsigned short>(k, 0)) > thr_bakground_L1 || ((subtraction_3_L1.at<unsigned short>(k, 0)) < 0 && (average_3_L1.at<unsigned short>(k, 0)) > thr_bakground_L1 - 1000))
					{
						intensity_L1[0] = 255;
						intensity_L1[1] = 255;
						intensity_L1[2] = 255;
					}

					tmp_colormat_L1.at<Vec3b>(Point(0, k)) = intensity_L1;
				}
			}
			tmp_colormat_L1(cv::Rect(0, 0, 1, WIN_HEIGHT)).copyTo(default_big_for_sen_L1(cv::Rect(big_index_L1, 0, 1, WIN_HEIGHT)));
		}
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 fill big mats");
		logging::INFO(e.what());
	}

}
void L1_Fill_based_mode_direction()
{
	if (direct == 1)
	{
		//logging::INFO("auto : " + to_string(M_AUTO));
		if (M_RGB && !M_VARI && !M_SEN)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

			}
			else
			{
				default_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}
		}
		else if (M_RGB && M_VARI)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_for_vari_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_for_vari_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));
			}
			else
			{
				default_big_for_vari_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}

		}
		else if (M_SEN)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_for_sen_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_for_sen_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));
			}
			else
			{
				default_big_for_sen_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}
		}
		else if (M_AUTO)
		{
			if (blink)
			{
				if (Allowed_White_Line_Mode_L1)
				{
					blinke_rect = true;
					blinke_not_rect = false;
				}
				if (blinke_rect)
				{
					if (Index_Filler_L1 < WIN_WIDTH - 1)
					{
						//right
						default_big_for_rectangle_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
						//left
						default_big_for_rectangle_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

					}
					else
					{
						default_big_for_rectangle_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
					}
				}
				if (blinke_not_rect)
				{
					if (Index_Filler_L1 < WIN_WIDTH - 1)
					{
						//right
						default_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
						//left
						default_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

					}
					else
					{
						default_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
					}
				}
			}
			else
			{
				if (Index_Filler_L1 < WIN_WIDTH - 1)
				{
					//right
					default_big_for_rectangle_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
					//left
					default_big_for_rectangle_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				}
				else
				{
					default_big_for_rectangle_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
				}
			}
		}
	}
	else if (direct == 2)
	{
		if (M_RGB && !M_VARI && !M_SEN)
		{

			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

			}
			else
			{
				default_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}
		}
		else if (M_RGB && M_VARI)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_for_vari_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_for_vari_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));
			}
			else
			{
				default_big_for_vari_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}

		}
		else if (M_SEN)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_for_sen_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_for_sen_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));
			}
			else
			{
				default_big_for_sen_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}
		}
		else if (M_AUTO)
		{
			if (blink)
			{
				if (do_blink)
				{
					if (Index_Filler_L1 < WIN_WIDTH - 1)
					{
						//right
						default_big_for_rectangle_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
						//left
						default_big_for_rectangle_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

					}
					else
					{
						default_big_for_rectangle_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
					}
				}
				else
				{
					if (Index_Filler_L1 < WIN_WIDTH - 1)
					{
						//right
						default_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
						//left
						default_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

					}
					else
					{
						default_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
					}
				}
			}
			else
			{
				if (Index_Filler_L1 < WIN_WIDTH - 1)
				{
					//right
					default_big_for_rectangle_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
					//left
					default_big_for_rectangle_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				}
				else
				{
					default_big_for_rectangle_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
				}
			}

		}
		flip(default_small_L1, default_small_L1, 1);
		default_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_VIEW)).copyTo(view_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_VIEW)));
	}

	if (Direction_Convoyer == 21)
	{
		//putText(view_small_L1, "R E A L", Point(670, 100), 5, 2.8, Scalar(0, 0, 0), 3, 8, false);
	}
	if (Direction_Convoyer == 12)
	{
		//putText(view_small_L1, "B U F F E R", Point(670, 100), 5, 2.8, Scalar(0, 0, 0), 3, 8, false);
	}

}
void L1_Fill_based_mode_direction_semi()
{
	if (direct == 1)
	{
		//logging::INFO("auto : " + to_string(M_AUTO));
		if (M_RGB && !M_VARI && !M_SEN)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

			}
			else
			{
				default_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}
		}
		else if (M_RGB && M_VARI)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_for_vari_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_for_vari_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));
			}
			else
			{
				default_big_for_vari_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}

		}
		else if (M_SEN)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_for_sen_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_for_sen_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));
			}
			else
			{
				default_big_for_sen_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}
		}
		else if (M_AUTO)
		{
			if (blink)
			{
				if (Allowed_White_Line_Mode_L1)
				{
					blinke_rect = true;
					blinke_not_rect = false;
				}
				if (blinke_rect)
				{
					if (Index_Filler_L1 < WIN_WIDTH - 1)
					{
						//right
						default_big_for_rectangle_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
						//left
						default_big_for_rectangle_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

					}
					else
					{
						default_big_for_rectangle_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
					}
				}
				if (blinke_not_rect)
				{
					if (Index_Filler_L1 < WIN_WIDTH - 1)
					{
						//right
						default_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
						//left
						default_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

					}
					else
					{
						default_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
					}
				}
			}
			else
			{
				if (Index_Filler_L1 < WIN_WIDTH - 1)
				{
					//right
					default_big_for_rectangle_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
					//left
					default_big_for_rectangle_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				}
				else
				{
					default_big_for_rectangle_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(view_small_semi_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
				}
			}
		}
	}
	else if (direct == 2)
	{
		if (M_RGB && !M_VARI && !M_SEN)
		{

			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

			}
			else
			{
				default_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}
		}
		else if (M_RGB && M_VARI)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_for_vari_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_for_vari_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));
			}
			else
			{
				default_big_for_vari_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}

		}
		else if (M_SEN)
		{
			if (Index_Filler_L1 < WIN_WIDTH - 1)
			{
				//right
				default_big_for_sen_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
				//left
				default_big_for_sen_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));
			}
			else
			{
				default_big_for_sen_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
			}
		}
		else if (M_AUTO)
		{
			if (blink)
			{
				if (do_blink)
				{
					if (Index_Filler_L1 < WIN_WIDTH - 1)
					{
						//right
						default_big_for_rectangle_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
						//left
						default_big_for_rectangle_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

					}
					else
					{
						default_big_for_rectangle_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
					}
				}
				else
				{
					if (Index_Filler_L1 < WIN_WIDTH - 1)
					{
						//right
						default_big_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
						//left
						default_big_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

					}
					else
					{
						default_big_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
					}
				}
			}
			else
			{
				if (Index_Filler_L1 < WIN_WIDTH - 1)
				{
					//right
					default_big_for_rectangle_L1(cv::Rect(0, 0, Index_Filler_L1 + 1, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(WIN_WIDTH - 1 - Index_Filler_L1, Y_OFFSET, Index_Filler_L1 + 1, WIN_HEIGHT)));
					//left
					default_big_for_rectangle_L1(cv::Rect(BIG_WIN_WIDTH - 1 - (WIN_WIDTH - (Index_Filler_L1 + 1)) + 1, 0, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH - (Index_Filler_L1 + 1), WIN_HEIGHT)));

				}
				else
				{
					default_big_for_rectangle_L1(cv::Rect(Index_Filler_L1 - WIN_WIDTH + 1, 0, WIN_WIDTH, WIN_HEIGHT)).copyTo(default_small_L1(cv::Rect(0, Y_OFFSET, WIN_WIDTH, WIN_HEIGHT)));
				}
			}

		}
		flip(default_small_L1, default_small_L1, 1);
		default_small_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_VIEW)).copyTo(view_small_semi_L1(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT_VIEW)));
	}
}
void L1_Fill_Viewer()
{


	while (1)
	{
		try
		{
			if (move_state && Rendering)
			{

				///////////////////////////////////////////////////////////////////////////// L1  >>>  L2
				//logging::INFO("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
				//logging::INFO("L1_Filler_canbe_Called: " + to_string(L1_Filler_canbe_Called));
				//logging::INFO("big_index_all_L1: " + to_string(big_index_all_L1));
				//logging::INFO("Index_Filler_L1: " + to_string(Index_Filler_L1));

				if (L1_Filler_canbe_Called)
				{
					if (Index_Filler_all_L1 < big_index_all_L1 - (object_width_thereshold_L1 + 5))
					{
						//logging::INFO(">>>>>>>>>>>>>>>>>>>   L1 INSERTER");
						//QueryPerformanceFrequency(&FT11);
						//QueryPerformanceCounter(&ST11);

						//logging::INFO("10000_L1");

						L1_Timer_Start();
						/////////////////////////////////////////
						Index_Filler_L1 += 1;
						Index_Filler_all_L1 += 1;
						if (Index_Filler_L1 == BIG_WIN_WIDTH)
						{
							Index_Filler_L1 = 0;
						}
						L1_Fill_based_mode_direction();
						/////////////////////////////////////////
						if (Direction_Convoyer == 12)
						{
							L1_Timer_Stop(int(double(LOOP_INTERVAL)));// *nearly));
							//logging::INFO("20000_L1");
						}

						//QueryPerformanceCounter(&SP11);
						//DTT11 = (double)(SP11.QuadPart - ST11.QuadPart) * 1000.0 / (double)FT11.QuadPart;
						//logging::INFO("L1_INSERTER_TIME: " + to_string(DTT11));
					}
				}
				else
				{
					if (Direction_Convoyer == 12)
					{
						if (big_index_L1 - Index_Filler_L1 > object_width_thereshold_L1 + 6 && L1_Num_of_Bgn_Points == L2_Num_of_Bgn_Points)
						{
							//logging::INFO("L1 >> Index_Filler_L1: " + to_string(Index_Filler_L1) + " big_index_L1: " + to_string(big_index_L1) + " diff: " + to_string(big_index_L1 - Index_Filler_L1));
							//L1_Filler_canbe_Called = true;
						}
					}
				}
			}
			else
			{
				std::this_thread::sleep_for(5ms);
			}
		}
		catch (const std::exception& e)
		{
			logging::INFO("error // filling    index_filler l1: " + to_string(Index_Filler_L1));
			logging::INFO(e.what());
		}
	}
}
void L1_Processing_Blk()
{
	L1_Processing = true;

	L1_Blink_Check();
	L1_Reset_Flags_Vars();
	L1_Shift_Mats();
	L1_Insert_Lines();
	L1_Filter();
	L1_Fill_3c_Mats();
	L1_CL_Line_Generator();
	L1_White_Object_Line_Detector();
	L1_Check_Gen_Status();
	//logging::INFO("L1_x_gen_is_off: " + to_string(L1_x_gen_is_off));


	if (is_object_line_L1)
	{
		L1_current_line_status = 2;
		L1_W_CNT = 0;
		L1_O_CNT += 1;
		L1_U_CNT = 0;
	}
	if (is_white_line_L1)
	{
		L1_current_line_status = 1;
		L1_W_CNT += 1;
		L1_O_CNT = 0;
		L1_U_CNT = 0;
	}
	if (!is_white_line_L1 && !is_object_line_L1)
	{
		L1_current_line_status = 0;
		L1_W_CNT = 0;
		L1_O_CNT = 0;
		L1_U_CNT += 1;
	}

	if (L1_current_line_status != L1_previous_line_status)
	{
		if (is_object_line_L1)
		{
			//logging::INFO("L1 >> OBJECT  W:"+to_string(L1_W_CNT)+" O:"+to_string(L1_O_CNT) + " U:" + to_string(L1_U_CNT));
		}
		if (is_white_line_L1)
		{
			//logging::INFO("L1 >> WHITE  W:" + to_string(L1_W_CNT) + " O:" + to_string(L1_O_CNT) + " U:" + to_string(L1_U_CNT));
		}
		if (!is_white_line_L1 && !is_object_line_L1)
		{
			//logging::INFO("L1 >>  NO WHITE NO OBJECT  W:" + to_string(L1_W_CNT) + " O:" + to_string(L1_O_CNT) + " U:" + to_string(L1_U_CNT));
		}
	}

	L1_previous_line_status = L1_current_line_status;

	if (!is_white_line_L1 && !is_object_line_L1)
	{
		//ignore
		//logging::INFO("L1: ignore");
	}
	else
	{
		if (is_white_line_L1)
		{
			// >>>>>>>>>>>>>>> MAN INJA BAYAD PAYANE JESM RA TASHKHIS DAHAM
			L1_PROCESS_WHITE_LINE(); 
		}
		if (is_object_line_L1)
		{
			// >>>>>>>>>>>>>>> MAN INJA BAYAD SHOROOE JESM RA TASHKHIS DAHAM
			First_Run = false;
			L1_PROCESS_OBJECT_LINE();
			//logging::INFO("L1:OBJECT");
		}

		if (is_object_line_L1 || (is_white_line_L1 && Allowed_White_Line_Mode_L1))//(is_white_line_L1 && White_Lines_after_End_Obj_L1  < Allowed_White_Lines_after_Object))
		{
			// >>>>>>>>>>>>>>> HAMEYE OBJECT KINHA PROCESS MISHAVAND
			// >>>>>>>>>>>>>>> WHITE LINHA TA JAEE PROCESS MISHAVAND K ALLOWED BASHAND
			L1_Process_White_Object_Line();
			L1_Fill_Big_Mats();
		}
		//if (!is_white_line_L1 && !is_object_line_L1)
		//{
		//	Allowed_White_Line_Mode_L1 = true;// auto mode should be viewed if !white_line and !object_line
		//}
	}

	L1_Processing = false;
}
void L1_List_Obj_Filler()
{
	string Log_str = "";
	try
	{
		//logging::INFO("L1 >> SAVE_1");
		L1_List_Obj_Filler_is_busy = true;

		L1_OPP += 1;
		if (L1_OPP == Max_Objects_FB)
		{
			L1_OPP -= Max_Objects_FB;
		}

		Log_str += "1-";


		//logging::INFO("..........................................................SSSSSS");
		//New_Obj_from_L1 = false;
		int point_start_read = Instance_Obj_L1.start_point_object;
		int point_stop_read = Instance_Obj_L1.stop_point_object;
		int width_current_object = Instance_Obj_L1.Width_Obj;


		//logging::INFO(">>>>>>>>>> NEW OBJECT IS WRITING ON L1_OPP: " + to_string(L1_OPP));
		//logging::INFO("ST: " + to_string(point_start_read)
		//	+ " SP: " + to_string(point_stop_read)
		//	+ " W: " + to_string(width_current_object));

		L1_List_of_Obj_MAT[L1_OPP].start_point_object = Instance_Obj_L1.start_point_object;
		L1_List_of_Obj_MAT[L1_OPP].stop_point_object = Instance_Obj_L1.stop_point_object;
		L1_List_of_Obj_MAT[L1_OPP].Width_Obj = Instance_Obj_L1.Width_Obj;
		L1_List_of_Obj_MAT[L1_OPP].Time_Scan = Instance_Obj_L1.Time_Scan;
		L1_List_of_Obj_MAT[L1_OPP].validity = true;
		L1_List_of_Obj_MAT[L1_OPP].Lowenergy_mat(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)) = cv::Scalar(0xffff);
		L1_List_of_Obj_MAT[L1_OPP].Highenergy_mat(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)) = cv::Scalar(0xffff);
		L1_List_of_Obj_MAT[L1_OPP].RGB_mat(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)) = Scalar(255, 255, 255);
		L1_List_of_Obj_MAT[L1_OPP].RGB_RECT_mat(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)) = Scalar(255, 255, 255);
		L1_List_of_Obj_MAT[L1_OPP].Low_raw(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)) = cv::Scalar(0xffff);
		L1_List_of_Obj_MAT[L1_OPP].High_raw(cv::Rect(0, 0, WIN_WIDTH, WIN_HEIGHT)) = cv::Scalar(0xffff);

		//logging::INFO("L1 >> SAVE_3");

		Log_str += "2-";

		L1_detction_exp_drg_habs_FORCE();

		//logging::INFO("L1 >> SAVE_4");

		Log_str += "3-";

		/*logging::INFO("ST: " + to_string(point_start_read)
			+ " SP: " + to_string(point_stop_read)
			+ " W: " + to_string(width_current_object));*/

		if (width_current_object <= WIN_WIDTH)
		{
			if (point_stop_read < point_start_read)
			{
				Log_str += "4-";


				int lft_wid = point_stop_read + 1;//252
				int rit_wid = width_current_object - lft_wid;//142
				int edge_pnt_big = BIG_WIN_WIDTH - 1 - rit_wid + 1;//2855
				int edge_pnt = rit_wid;//142

				lowenergy_big_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].Lowenergy_mat(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				highenergy_big_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].Highenergy_mat(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				default_big_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].RGB_mat(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				default_big_for_rectangle_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].RGB_RECT_mat(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				lowenergy_big_input_default_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].Low_raw(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				highenergy_big_input_default_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].High_raw(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));

				Log_str += "5-";


				lowenergy_big_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].Lowenergy_mat(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				highenergy_big_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].Highenergy_mat(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				default_big_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].RGB_mat(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				default_big_for_rectangle_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].RGB_RECT_mat(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				lowenergy_big_input_default_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].Low_raw(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				highenergy_big_input_default_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].High_raw(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));

				Log_str += "6-";
			}
			else
			{
				Log_str += "7-";
				lowenergy_big_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].Lowenergy_mat(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				highenergy_big_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].Highenergy_mat(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				default_big_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].RGB_mat(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				default_big_for_rectangle_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].RGB_RECT_mat(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				lowenergy_big_input_default_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].Low_raw(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				highenergy_big_input_default_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_List_of_Obj_MAT[L1_OPP].High_raw(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				Log_str += "8-";
			}
		}
		else
		{
			L1_List_of_Obj_MAT[L1_OPP].Width_Obj = WIN_WIDTH;
			//logging::INFO(+" W_NEW: " + to_string(L1_List_of_Obj_MAT[L1_OPP].Width_Obj));
			cv::Mat L1_Lowenergy_mat = cv::Mat(WIN_HEIGHT, width_current_object, CV_16UC1, cv::Scalar(0xffff));
			cv::Mat L1_Highenergy_mat = cv::Mat(WIN_HEIGHT, width_current_object, CV_16UC1, cv::Scalar(0xffff));
			cv::Mat L1_RGB_mat = cv::Mat(WIN_HEIGHT, width_current_object, CV_8UC3, Scalar(255, 255, 255));
			cv::Mat L1_RGB_RECT_mat = cv::Mat(WIN_HEIGHT, width_current_object, CV_8UC3, Scalar(255, 255, 255));
			cv::Mat L1_Low_raw = cv::Mat(WIN_HEIGHT, width_current_object, CV_16UC1, cv::Scalar(0xffff));
			cv::Mat L1_High_raw = cv::Mat(WIN_HEIGHT, width_current_object, CV_16UC1, cv::Scalar(0xffff));

			if (point_stop_read < point_start_read)
			{
				Log_str += "44-";

				int lft_wid = point_stop_read + 1;//252
				int rit_wid = width_current_object - lft_wid;//142
				int edge_pnt_big = BIG_WIN_WIDTH - 1 - rit_wid + 1;//2855
				int edge_pnt = rit_wid;//142

				lowenergy_big_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_Lowenergy_mat(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				highenergy_big_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_Highenergy_mat(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				default_big_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_RGB_mat(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				default_big_for_rectangle_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_RGB_RECT_mat(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				lowenergy_big_input_default_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_Low_raw(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));
				highenergy_big_input_default_L1(cv::Rect(0, 0, lft_wid, WIN_HEIGHT)).copyTo(L1_High_raw(cv::Rect(edge_pnt, 0, lft_wid, WIN_HEIGHT)));

				Log_str += "55-";

				/*for (int j = 0; j < rit_wid; j++)
				{
					for (int k = 0; k < WIN_HEIGHT; k++)
					{

						L1_List_of_Obj_MAT[L1_OPP].Lowenergy_mat.at<unsigned short>(k, j) = lowenergy_big_L1.at<unsigned short>(k, j + edge_pnt_big);
						L1_List_of_Obj_MAT[L1_OPP].Highenergy_mat.at<unsigned short>(k, j) = highenergy_big_L1.at<unsigned short>(k, j + edge_pnt_big);
						L1_List_of_Obj_MAT[L1_OPP].RGB_mat.at<Vec3b>(k, j) = default_big_L1.at<Vec3b>(k, j + edge_pnt_big);
						L1_List_of_Obj_MAT[L1_OPP].RGB_RECT_mat.at<Vec3b>(k, j) = default_big_for_rectangle_L1.at<Vec3b>(k, j + edge_pnt_big);
						L1_List_of_Obj_MAT[L1_OPP].Low_raw.at<unsigned short>(k, j) = lowenergy_big_input_default_L1.at<unsigned short>(k, j + edge_pnt_big);
						L1_List_of_Obj_MAT[L1_OPP].High_raw.at<unsigned short>(k, j) = highenergy_big_input_default_L1.at<unsigned short>(k, j + edge_pnt_big);
					}
				}*/

				lowenergy_big_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_Lowenergy_mat(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				highenergy_big_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_Highenergy_mat(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				default_big_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_RGB_mat(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				default_big_for_rectangle_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_RGB_RECT_mat(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				lowenergy_big_input_default_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_Low_raw(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));
				highenergy_big_input_default_L1(cv::Rect(edge_pnt_big, 0, rit_wid, WIN_HEIGHT)).copyTo(L1_High_raw(cv::Rect(0, 0, rit_wid, WIN_HEIGHT)));

				Log_str += "66-";
			}
			else
			{
				Log_str += "77-";
				lowenergy_big_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_Lowenergy_mat(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				highenergy_big_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_Highenergy_mat(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				default_big_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_RGB_mat(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				default_big_for_rectangle_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_RGB_RECT_mat(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				lowenergy_big_input_default_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_Low_raw(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				highenergy_big_input_default_L1(cv::Rect(point_start_read, 0, width_current_object, WIN_HEIGHT)).copyTo(L1_High_raw(cv::Rect(0, 0, width_current_object, WIN_HEIGHT)));
				Log_str += "88-";
			}

			//=========================================================================== Resize mat
			cv::resize(L1_Lowenergy_mat, L1_List_of_Obj_MAT[L1_OPP].Lowenergy_mat, cv::Size(WIN_WIDTH, WIN_HEIGHT), 0, 0, 5);
			cv::resize(L1_Highenergy_mat, L1_List_of_Obj_MAT[L1_OPP].Highenergy_mat, cv::Size(WIN_WIDTH, WIN_HEIGHT), 0, 0, 5);
			cv::resize(L1_RGB_mat, L1_List_of_Obj_MAT[L1_OPP].RGB_mat, cv::Size(WIN_WIDTH, WIN_HEIGHT), 0, 0, 5);
			cv::resize(L1_RGB_RECT_mat, L1_List_of_Obj_MAT[L1_OPP].RGB_RECT_mat, cv::Size(WIN_WIDTH, WIN_HEIGHT), 0, 0, 5);
			cv::resize(L1_Low_raw, L1_List_of_Obj_MAT[L1_OPP].Low_raw, cv::Size(WIN_WIDTH, WIN_HEIGHT), 0, 0, 5);
			cv::resize(L1_High_raw, L1_List_of_Obj_MAT[L1_OPP].High_raw, cv::Size(WIN_WIDTH, WIN_HEIGHT), 0, 0, 5);

			Log_str += "99-";
			L1_Lowenergy_mat.release();
			L1_Highenergy_mat.release();
			L1_RGB_mat.release();
			L1_RGB_RECT_mat.release();
			L1_Low_raw.release();
			L1_High_raw.release();
		}

		//logging::INFO("..........................................................EEEEEE");
		//imshow("L1", L1_List_of_Obj_MAT[L1_OPP].RGB_mat);
		//waitKey(1);
		//logging::INFO("L1 >> SAVE_5");

		if (Direction_Convoyer == 21)
		{
			OBJCNT_OUT_AFTER_INSERT_TO_LIST += 1;
		}

		L1_AutoSave_Handle = true;
		L1_List_Obj_Filler_is_busy = false;
		Log_str += "9-";

		//logging::INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

		/*logging::INFO("L1 >> SAVE_IN_LIST || ST: " + to_string(point_start_read)
			+ " SP: " + to_string(point_stop_read)
			+ " W: " + to_string(width_current_object)
			+ " L1_OPP:" + to_string(L1_OPP)
			+ " OBJ_OUT_AFT_INS_TO_LIST:" + to_string(OBJCNT_OUT_AFTER_INSERT_TO_LIST));*/
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // List_Obj_Filler_L1" + Log_str);
		logging::INFO(e.what());
	}

}
void L1_Blink_Check()
{
	if (blink)
	{
		if (count_line_index_L1 % blink_speed == 0)
		{
			if (blinke_rect)
			{
				blinke_rect = false;
				blinke_not_rect = true;
			}
			else
			{
				blinke_rect = true;
				blinke_not_rect = false;
			}
		}
	}
}
void L1_PROCESS_WHITE_LINE()
{
	try
	{
		AV_40_Average_MonoColorLine_L1 = 0;

		if (Direction_Convoyer == 12)
		{
			if (Allowed_White_Line_Mode_L1)
			{
				///////////////////////////////////////////
				//BE SHARTI WHITE LINE HA RA MISHOMARIM K 
				//OBJECT TAMAM SHODE BASHAD VA FLAG 
				//MARBOOTE SET SHODE BASHAD.
				///////////////////////////////////////////
				White_Lines_after_End_Obj_L1 += 1;

				///////////////////////////////////////////
				// lahze e k flag new_obj_is_come true ast
				// va chan line sefid rad shode ast be mani
				// payan object ast   
				///////////////////////////////////////////

				if (White_Lines_after_End_Obj_L1 == THR_White_Lines_Verify_End_Obj_L1 && NEW_Obj_is_Coming_L1)
				{
					NEW_Obj_is_Coming_L1 = false;
					Object_Lines_from_Bgn_Obj_L1 = 0;
					L1_Set_Ending_Object_Value();

					L1_Write_Object_Information_BUFFER();
				}
			}
			else
			{
				// in white linha bayad ignore shavand
			}
		}

		if (Direction_Convoyer == 21)
		{
			//logging::INFO("L1 >> L1_Has_Seen_Enough_Width_as_reference: "+to_string(L1_Has_Seen_Enough_Width_as_reference));
			if (L1_Has_Seen_Enough_Width_as_reference) // L1 dar halate entezar baraye didan e object line NIST
			{
				if (Allowed_White_Line_Mode_L1)
				{
					White_Lines_after_End_Obj_L1 += 1;
				}
				else
				{
					//IGNORE WHITE LINE
				}

				NEW_Obj_is_Coming_L1 = false;
				Object_Lines_from_Bgn_Obj_L1 = 0;
			}
			if (!L1_Has_Seen_Enough_Width_as_reference)
			{
				White_Lines_after_End_Obj_L1 += 1;

				if (NEW_Obj_is_Coming_L1)
				{
					int Reference_Width = UPDATED_WIDTH_FROM_L2_VEC[Chk_on_UPDATED_WIDTH_FROM_L2_VEC_by_L1];
					if (Reference_Width == -1)
					{
						// hanooz update az samte L2 nayamde
					}
					else
					{
						Current_Width_Instance = Compute_Current_Width_Instance(Begining_Object_L1, big_index_L1);
						//logging::INFO("L1 >> Current_Width_Instance: " + to_string(Current_Width_Instance + " Reference_Width: " + to_string(Reference_Width));

						if (Current_Width_Instance >= Reference_Width)
						{
							NEW_Obj_is_Coming_L1 = false;
							Object_Lines_from_Bgn_Obj_L1 = 0;
							L1_Set_Ending_Object_Value();
							//logging::INFO("L1 >> from_white_processing");
							logging::INFO("L1 >> W_SECTION_L1_Seen || W_REF: " + to_string(Current_Width_Instance));
							L1_Write_Object_Information_REAL();

							Chk_on_UPDATED_WIDTH_FROM_L2_VEC_by_L1 += 1;
							L1_Has_Seen_Enough_Width_as_reference = true;


						}
					}
				}
			}

			//if (Allowed_White_Line_Mode_L1)
			//{
			//	//////////////////////////////////////////
			//	// nezarate width felie L1 va moghyese ba 
			//	// width L2...BAYAD BAHAM BARABAR BASHAND 
			//	//////////////////////////////////////////
			//	White_Lines_after_End_Obj_L1 += 1;
			//	if (L1_Has_Seen_Enough_Width_as_reference) // L1 dar halate entezar baraye didan e object line NIST
			//	{
			//		NEW_Obj_is_Coming_L1 = false;
			//		Object_Lines_from_Bgn_Obj_L1 = 0;
			//		//niaz nist kari bokonim
			//	}
			//	// L1 dar halate entezar baraye didan e object line AST ta width jesme feli 
			//	// ba width jesm refrence barabar shavad va dar in halat faghat bayad nezaregare 
			//	// width bashham ta betavananm payane jesm ra elam konam
			//	if (!L1_Has_Seen_Enough_Width_as_reference)
			//	{
			//		if (NEW_Obj_is_Coming_L1)
			//		{
			//			int Reference_Width = UPDATED_WIDTH_FROM_L2_VEC[Chk_on_UPDATED_WIDTH_FROM_L2_VEC_by_L1];
			//			if (Reference_Width == -1)
			//			{
			//				// hanooz update az samte L2 nayamde
			//			}
			//			else
			//			{
			//				Current_Width_Instance = Compute_Current_Width_Instance(Begining_Object_L1, big_index_L1);
			//				if (Current_Width_Instance >= Reference_Width)
			//				{
			//					NEW_Obj_is_Coming_L1 = false;
			//					Object_Lines_from_Bgn_Obj_L1 = 0;
			//					L1_Set_Ending_Object_Value();
			//					logging::INFO("L1 >> from_white_processing");
			//					L1_Write_Object_Information_REAL();
			//					Chk_on_UPDATED_WIDTH_FROM_L2_VEC_by_L1 += 1;
			//					L1_Has_Seen_Enough_Width_as_reference = true;
			//					logging::INFO("L1 >> L1_Has_Seen_Enough_Width_as_reference: " + to_string(L1_Has_Seen_Enough_Width_as_reference));
			//				}
			//			}
			//		}
			//	}
			//}
			//else
			//{
			//	// in white linha bayad ignore shavand
			//}
		}

		L1_Share_White_Block_Proccessing();
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 process white line");
		logging::INFO(e.what());
	}
}
void L1_PROCESS_OBJECT_LINE()
{
	try
	{
		AV_40_Average_MonoColorLine_L1 += Average_MonoColorLine_L1;

		L1_x_gen_could_be_off = 0;
		L2_x_gen_could_be_off = 0;

		if (Direction_Convoyer == 12)
		{
			//logging::INFO("OBJECT");

			Object_Lines_from_Bgn_Obj_L1 += 1;

			//logging::INFO("Object_Lines_from_Bgn_Obj_L1: " + to_string(Object_Lines_from_Bgn_Obj_L1));

			///////////////////////////////////////////////
			// Tashkhis Shooroo e Object 			 
			///////////////////////////////////////////////


			if (Object_Lines_from_Bgn_Obj_L1 == object_width_thereshold_L1 && !NEW_Obj_is_Coming_L1)
			{
				AV_40_Average_MonoColorLine_L1 = AV_40_Average_MonoColorLine_L1 / double(object_width_thereshold_L1);

				//if (AV_40_Average_MonoColorLine_L1 > 0.0)
				//{

					NEW_Obj_is_Coming_L1 = true;
					White_Lines_after_End_Obj_L1 = 0;
					L1_Set_Begining_Object_Value();
					Allowed_White_Line_Mode_L1 = true;

					logging::INFO("L1 >> ======================================================");
					logging::INFO("L1 >> * BEGINING(BUFFER) >> BO_L1: " + to_string(Begining_Object_L1) +
						" IF_L1:: " + to_string(Index_Filler_L1) +
						//" AV_40_L1: " + to_string(AV_40_Average_MonoColorLine_L1) +
						" L1_NBgn: " + to_string(L1_Num_of_Bgn_Points) +
						" L2_NBgn: " + to_string(L2_Num_of_Bgn_Points)
					);

				//}
				///////////////////////////////////////////
				//  mohasebe fasele entehaye jesme akhar az 
				//  ebtedaye jesme dar hale vorood
				///////////////////////////////////////////

				//if (One_Obj_Passed_L1)
				//{
				//	L1_Compute_Number_Lines_Two_Object();
				//}
			}
		}

		if (Direction_Convoyer == 21)
		{

			// L1 dar halate entezar baraye didan e object line NIST va L1 migooyad man 
			// baraye in object line haye jadid barnameye khasi nadaram
			// pas mitavanam montazere shooroo e object jadadi ra tashkhis daham

			if (L1_Has_Seen_Enough_Width_as_reference)
			{
				if (L1_Num_of_Bgn_Points < L2_Num_of_Bgn_Points)
				{
					Object_Lines_from_Bgn_Obj_L1 += 1;

					if (!NEW_Obj_is_Coming_L1)
					{
						if (Object_Lines_from_Bgn_Obj_L1 == object_width_thereshold_L1)
						{
							AV_40_Average_MonoColorLine_L1 = AV_40_Average_MonoColorLine_L1 / double(object_width_thereshold_L1);

							//if (AV_40_Average_MonoColorLine_L1 > 10.0)
							//{
								NEW_Obj_is_Coming_L1 = true;
								White_Lines_after_End_Obj_L1 = 0;
								L1_Set_Begining_Object_Value();

								L1_Has_Seen_Enough_Width_as_reference = false;
								//logging::INFO("L1 >> L1_Has_Seen_Enough_Width_as_reference: " + to_string(L1_Has_Seen_Enough_Width_as_reference));
								logging::INFO("L1 >> ======================================================");
								logging::INFO("L1 >> * BEGINING(REAL) >> BO_L1: " + to_string(Begining_Object_L1) +
									" IF_L1:: " + to_string(Index_Filler_L1) +
									//" AV_40_L1: " + to_string(AV_40_Average_MonoColorLine_L1) +
									" L1_NBgn: " + to_string(L1_Num_of_Bgn_Points) +
									" L2_NBgn: " + to_string(L2_Num_of_Bgn_Points)
								);
						//	}
							/*if (big_index_L2 > Index_Filler_L2)
							{
								difference_big_index_index_filler_L2 = big_index_L2 - Index_Filler_L2 + 1;
							}
							else
							{
								difference_big_index_index_filler_L2 = BIG_WIN_WIDTH - Index_Filler_L2 + big_index_L2 + 1;
							}
							if (difference_big_index_index_filler_L2 < 340)
							{
								nearly = 1.0;
							}
							else
							{
								double tet = (2025.0 / difference_big_index_index_filler_L2);
								nearly = tet / 6.0;
							}
							logging::INFO("L1 >> nearly : " + to_string(nearly));*/
						}
					}
				}
			}

			// L1 dar halate entezar baraye didan e object line AST ta width jesme feli 
			// ba width jesm refrence barabar shavad va dar in halat faghat bayad nezaregare 
			// width bashham ta betavananm payane jesm ra elam konam
			if (!L1_Has_Seen_Enough_Width_as_reference)
			{
				if (NEW_Obj_is_Coming_L1)
				{
					int Reference_Width = UPDATED_WIDTH_FROM_L2_VEC[Chk_on_UPDATED_WIDTH_FROM_L2_VEC_by_L1];
					if (Reference_Width == -1)
					{
						// hanooz update az samte L2 nayamde
					}
					else
					{
						Current_Width_Instance = Compute_Current_Width_Instance(Begining_Object_L1, big_index_L1);
						if (Current_Width_Instance >= Reference_Width)
						{
							NEW_Obj_is_Coming_L1 = false;
							Object_Lines_from_Bgn_Obj_L1 = 0;
							L1_Set_Ending_Object_Value();
							//logging::INFO("L1 >> from_object_processing");
							logging::INFO("L1 >> O_SECTION_L1_Seen || W_REF: " + to_string(Current_Width_Instance));
							L1_Write_Object_Information_REAL();

							Chk_on_UPDATED_WIDTH_FROM_L2_VEC_by_L1 += 1;
							L1_Has_Seen_Enough_Width_as_reference = true;
							

						}
					}
				}
			}
		}

	}
	catch (const std::exception& e)
	{
		logging::INFO("error // l1 process object line");
		logging::INFO(e.what());
	}
}
void ProcessLine_L1(unsigned short gs_line_1_input[])
{
	try
	{
		if (move_state && Scan_State == 1000)
		{

			if (should_grab_line)
			{
				//logging::INFO(">>>>>>>>>>>>>>>>>>>   PL1 called");
				//QueryPerformanceFrequency(&FT1);
				//QueryPerformanceCounter(&ST1);
				//-----------------------------------------------
#ifdef _XRAY_100100D_EXPORTS

				for (int i = 0; i < 2304; i++)
				{

					if ((i % 64) == 0 && (i > 0))
					{
						HLP_1_L1.at<unsigned short>(i, 0) = double((gs_line_1_input[i - 1] + gs_line_1_input[i + 1]) / 2.0);
					}
					else
					{
						HLP_1_L1.at<unsigned short>(i, 0) = double(gs_line_1_input[i]);
					}

				}
				cv::resize(HLP_1_L1, HLP_2_L1, cv::Size(1, 1920), 0, 0, 5);
#endif
//logging::INFO(">>>>>>>>>>>>>>>>>>>  before  PL1 called");
#ifdef _XRAY_7555D_EXPORTS

				//double ave_l1 = 0.0;
				for (int i = 0; i < 1536; i++)
				{

					if ((i % 64) == 0 && (i > 0))
					{
						HLP_1_L1.at<unsigned short>(i, 0) = double((gs_line_1_input[i - 1] + gs_line_1_input[i + 1]) / 2.0);
					}
					else
					{
						HLP_1_L1.at<unsigned short>(i, 0) = double(gs_line_1_input[i]);
					}

					//ave_l1+= double(gs_line_1_input[i]);

				}
				//ave_l1 = ave_l1 / 1536;
				//logging::INFO("AVE_L1 : " + to_string(ave_l1));

				HLP_1_L1(cv::Rect(0, 0, 1, 1536)).copyTo(HLP_2_L1(cv::Rect(0, 0, 1, 1536)));
#endif
				//logging::INFO(">>>>>>>>>>>>>>>>>>>  after  PL1 called");
#ifdef _XRAY_6040D_EXPORTS

				for (int i = 0; i < WIN_HEIGHT * 2; i++)
				{
					HLP_1_L1.at<unsigned short>(i, 0) = double(gs_line_1_input[i]);
				}

#endif
				if (L1_stopping_mode)
				{
					L1_Offset_flag_while_stop_cnt += 1;

					if (L1_Offset_flag_while_stop_cnt <= L1_Offset_flag_while_stop_thr)
					{
						logging::INFO("L1 >.> PL1 >> GL:1 >> STOPPING >> IN STOP_PRIOD      Number_of_lines_calling_between_TWO_Lshape: "+to_string(Number_of_lines_calling_between_TWO_Lshape));
						//logging::INFO("L1_offset_flag_while_stop_cnt: " + to_string(L1_Offset_flag_while_stop_cnt));

						if (Direction_Convoyer == 21)
						{
							// L1 IS REAL

							if (L2_Num_of_Bgn_Points >= 1)
							{
								L1_Processing_Blk();
							}
						}
						else
						{

							// L1 IS BUFFER
							L1_Processing_Blk();

							// hala k Tunnel_Direction_Condition_Should_be_checked_BUFFER = false shode ast
							Number_of_lines_calling_between_TWO_Lshape += 1;

						}
					}
					else
					{
						logging::INFO("L1 >.> PL1 >> GL:1 >> STOPPING >> IN STOP_PRIOD - IGNORED");
						//logging::INFO("L1_ignore");

						//ignore
					}
				}
				else
				{
					logging::INFO("L1 >.> PL1 >> GL:1 >> MOVING      Number_of_lines_calling_between_TWO_Lshape: " + to_string(Number_of_lines_calling_between_TWO_Lshape));
					if (Direction_Convoyer == 12)
					{
						//L1 BUFFER AST
						if (Tunnel_Direction_Condition_Should_be_checked_BUFFER)
						{
							///////////////////////////////////////////////////////////////  NEW CONDITION IN REVERSE MODE
							L1_Processing = true;

							L1_Blink_Check();
							L1_Reset_Flags_Vars();
							L1_Shift_Mats();
							L1_Insert_Lines();
							L1_Filter();
							L1_Fill_3c_Mats();
							L1_CL_Line_Generator();
							L1_White_Object_Line_Detector();
							L1_Check_Gen_Status();
							//L1_Xact_Rect_Check();

							if (is_white_line_L1)
							{
								L1_PROCESS_WHITE_LINE();

								Sum_White_Line_X_Condition += 1;

								L1_Process_White_Object_Line();

								L1_Fill_Big_Mats();
							}

							if (Sum_White_Line_X_Condition == 7)
							{
								Sum_White_Line_X_Condition = 0;
								Tunnel_Direction_Condition_Should_be_checked_BUFFER = false;
								logging::INFO("L1 >.> X State : Grabbing Lines Condition Have Been Stablished");
							}

							L1_Processing = false;
						}
						else
						{
							if (permit_processline)
							{
								L1_Processing_Blk();
							}

							// hala k Tunnel_Direction_Condition_Should_be_checked_BUFFER = false shode ast
							Number_of_lines_calling_between_TWO_Lshape += 1;
						}
					}
					else
					{
						//L1 REAL AST
						if (Tunnel_Direction_Condition_Should_be_checked_REAL)
						{
							if (!Tunnel_Direction_Condition_Should_be_checked_BUFFER)
							{
								if (Number_of_lines_calling_between_TWO_Lshape >= Number_of_lines_should_called_between_TWO_Lshape)
								{
									///////////////////////////////////////////////////////////////  NEW CONDITION IN REVERSE MODE
									L1_Processing = true;

									L1_Blink_Check();
									L1_Reset_Flags_Vars();
									L1_Shift_Mats();
									L1_Insert_Lines();
									L1_Filter();
									L1_Fill_3c_Mats();
									L1_CL_Line_Generator();
									L1_White_Object_Line_Detector();
									L1_Check_Gen_Status();
									//L1_Xact_Rect_Check();

									if (is_white_line_L1)
									{
										L1_PROCESS_WHITE_LINE();

										Sum_White_Line_X_Condition += 1;

										L1_Process_White_Object_Line();

										L1_Fill_Big_Mats();
									}

									if (Sum_White_Line_X_Condition == 4)
									{
										Sum_White_Line_X_Condition = 0;
										Tunnel_Direction_Condition_Should_be_checked_REAL = false;
										logging::INFO("L1 >.> X State : Grabbing Lines Condition Have Been Stablished");
									}

									L1_Processing = false;
								}

							}
						}
						else
						{
							if (permit_processline && L2_Num_of_Bgn_Points >= 1)
							{
								L1_Processing_Blk();
							}
						}


					}

					//if (Tunnel_Direction_Condition_Should_be_Checked)
					//{
					//	logging::INFO("L1 >.> PL1 >> GL:1 >> STARTING >> DIRECTION_CHANGED");
					//	if (Direction_Convoyer == 12)
					//	{
					//		//logging::INFO("PL1--->Tunnel_Direction_Condition_Should_be_Checked: " + to_string(Tunnel_Direction_Condition_Should_be_Checked));
					//		//L1L2_Insert_Lines_X_Condition();
					//		//L1L2_Check_X_Line_White_Obj();
					//		///////////////////////////////////////////////////////////////  NEW CONDITION IN REVERSE MODE
					//		L1_Processing = true;
					//		L1_Blink_Check();
					//		L1_Reset_Flags_Vars();
					//		L1_Shift_Mats();
					//		L1_Insert_Lines();
					//		L1_Filter();
					//		L1_Fill_3c_Mats();
					//		L1_CL_Line_Generator();
					//		L1_White_Object_Line_Detector();
					//		L1_Check_Gen_Status();
					//		//L1_Xact_Rect_Check();
					//		if (is_white_line_L1)
					//		{
					//			L1_PROCESS_WHITE_LINE();
					//			Sum_White_Line_X_Condition += 1;
					//			L1_Process_White_Object_Line();
					//			L1_Fill_Big_Mats();
					//		}
					//		if (Sum_White_Line_X_Condition == 4)
					//		{
					//			Sum_White_Line_X_Condition = 0;
					//			Tunnel_Direction_Condition_Should_be_Checked = false;
					//			logging::INFO("L1 >.> X State : Grabbing Lines Condition Have Been Stablished");
					//		}
					//		L1_Processing = false;
					//	}
					//	else
					//	{
					//		// logging::INFO("PL1 is waiting");
					//		// Process_line_l1 should be waited
					//		// chon dar in halat direction convoyer 21 ast va jesm ebtda bayad az jeloye L2 rad shavad ta ejaze sader shavad 
					//	}
					//}
					//else
					//{
					//	//==========================
					//	//  barresi stop vasat scan
					//	//==========================
					//	if (permit_processline)//vasat jesm nabashad
					//	{
					//		logging::INFO("L1 >.> PL1 >> GL:1 >> STARTING >> PERMIT:1");
					//		if (Direction_Convoyer == 21)
					//		{
					//			// L1 IS REAL
					//			if (L2_Num_of_Bgn_Points >= 1)
					//			{
					//				L1_Processing_Blk();
					//			}
					//		}
					//		else
					//		{
					//			// L1 IS BUFFER
					//			L1_Processing_Blk();
					//		}
					//	}
					//	else//vasat jesmm bashad
					//	{
					//		logging::INFO("L1 >.> PL1 >> GL:1 >> STARTING >> PERMIT:0");
					//	}
					//	//logging::INFO("L1_x_gen_could_be_off: " + to_string(L1_x_gen_could_be_off));
					//}
				}

			
			}
			else
			{
			    logging::INFO("L1 >.> PL1 >> GL:0");
				//ignor line
			}

			//QueryPerformanceCounter(&SP1);
				//DTT1 = (double)(SP1.QuadPart - ST1.QuadPart) * 1000.0 / (double)FT1.QuadPart;
				//logging::INFO("L1_PL_TIME: " + to_string(DTT1));
		}
	}
	catch (const std::exception& e)
	{
		logging::INFO("error // processline l1");
		logging::INFO(e.what());
	}

}