#ifndef HPP_PROCESSING
#define HPP_PROCESSING

namespace ModelView
{
	class Processing
	{
		public:
			enum Mode
			{
				Average,
				DownSampling,
				Filter,					// median, low pass, high pass, band pass
				Converting,		// log, dB 
			};
		
		private:
			Mode mode_;
			unsigned int average_;
			unsigned int downSampling_;
			unsigned int filter_;
		
		public:
			Processing(Mode, unsigned int, unsigned int, unsigned int);
			~Processing();
	};
}

#endif