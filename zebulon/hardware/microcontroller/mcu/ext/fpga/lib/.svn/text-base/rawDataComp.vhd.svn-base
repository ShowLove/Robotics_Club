library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.all;
--use IEEE.std_logic_unsigned.all;
--use IEEE.std_logic_signed.all;

package RawData is
	component rawDataComp is
	Port
	(
		TXD		: out std_logic := '1';
		RXD		: in std_logic;
		clock		: in	std_logic;
		
		mode		: in std_logic_vector (1 downto 0) := "00";
		dataMode : in std_logic := '0';
		
		analogDone : in std_logic;
		
		analog0	: in std_logic_vector (11 downto 0);
		analog1	: in std_logic_vector (11 downto 0);
		analog2	: in std_logic_vector (11 downto 0);
		analog3	: in std_logic_vector (11 downto 0);
		analog4	: in std_logic_vector (11 downto 0);
		
		ping0		: in std_logic;
		ping1		: in std_logic;
		ping2		: in std_logic;
		ping3		: in std_logic;
		ping4		: in std_logic;
		
		fft_index : out std_logic_vector (23 downto 0);
		fft0_re : out std_logic_vector (23 downto 0);
		fft0_im : out std_logic_vector (23 downto 0);
		fft1_re : out std_logic_vector (23 downto 0);
		fft1_im : out std_logic_vector (23 downto 0);
		fft2_re : out std_logic_vector (23 downto 0);
		fft2_im : out std_logic_vector (23 downto 0);
		fft3_re : out std_logic_vector (23 downto 0);
		fft3_im : out std_logic_vector (23 downto 0);
		fft4_re : out std_logic_vector (23 downto 0);
		fft4_im : out std_logic_vector (23 downto 0);
		
		fft_ready : out std_logic;
		fft_read : in std_logic
	);
	end component rawDataComp;
end package RawData;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.all;
use IEEE.std_logic_unsigned.all;
use work.RS232.all;

entity rawDataComp is
	Port
	(
		TXD		: out std_logic := '1';
		RXD		: in std_logic;
		clock		: in	std_logic;
		
		mode		: in std_logic_vector (1 downto 0) := "00";
		dataMode : in std_logic := '0';
		
		analogDone : in std_logic;
		
		analog0	: in std_logic_vector (11 downto 0);
		analog1	: in std_logic_vector (11 downto 0);
		analog2	: in std_logic_vector (11 downto 0);
		analog3	: in std_logic_vector (11 downto 0);
		analog4	: in std_logic_vector (11 downto 0);
		
		ping0		: in std_logic;
		ping1		: in std_logic;
		ping2		: in std_logic;
		ping3		: in std_logic;
		ping4		: in std_logic;
		
		fft_index : out std_logic_vector (23 downto 0);
		fft0_re : out std_logic_vector (23 downto 0);
		fft0_im : out std_logic_vector (23 downto 0);
		fft1_re : out std_logic_vector (23 downto 0);
		fft1_im : out std_logic_vector (23 downto 0);
		fft2_re : out std_logic_vector (23 downto 0);
		fft2_im : out std_logic_vector (23 downto 0);
		fft3_re : out std_logic_vector (23 downto 0);
		fft3_im : out std_logic_vector (23 downto 0);
		fft4_re : out std_logic_vector (23 downto 0);
		fft4_im : out std_logic_vector (23 downto 0);
		
		fft_ready : out std_logic;
		fft_read : in std_logic
	);
end rawDataComp;

architecture Behavioral of rawDataComp is

component fft
	port (
	clk: IN std_logic;
	start: IN std_logic;
	xn_re: IN std_logic_VECTOR(12 downto 0);
	xn_im: IN std_logic_VECTOR(12 downto 0);
	fwd_inv: IN std_logic;
	fwd_inv_we: IN std_logic;
	rfd: OUT std_logic;
	xn_index: OUT std_logic_VECTOR(8 downto 0);
	busy: OUT std_logic;
	edone: OUT std_logic;
	done: OUT std_logic;
	dv: OUT std_logic;
	unload: IN std_logic;
	xk_index: OUT std_logic_VECTOR(8 downto 0);
	xk_re: OUT std_logic_VECTOR(22 downto 0);
	xk_im: OUT std_logic_VECTOR(22 downto 0));
end component;

-- Synplicity black box declaration
attribute syn_black_box : boolean;
attribute syn_black_box of fft: component is true;


	signal dbInSig	:	std_logic_vector(7 downto 0);
	signal dbOutSig	:	std_logic_vector(7 downto 0);
	signal rdaSig	:	std_logic;
	signal tbeSig	:	std_logic;
	signal rdSig	:	std_logic;
	signal wrSig	:	std_logic;
	signal peSig	:	std_logic;
	signal feSig	:	std_logic;
	signal oeSig	:	std_logic;
	signal rstSig	:	std_logic	:= '0';
	
	type memory12 is array(0 to 511) of std_logic_vector(11 downto 0);
	type memory23 is array(0 to 255) of signed(22 downto 0);
	
	type array0 is array(0 to 4) of memory23;
	type array2 is array(0 to 4) of std_logic_vector(22 downto 0);
	type array3 is array(0 to 4) of memory12;
	
	type packet is array(0 to 10) of std_logic_vector(7 downto 0);

	signal AD0 : memory12;
	signal AD1 : memory12;
	signal AD2 : memory12;
	signal AD3 : memory12;
	signal AD4 : memory12;
	
	type memory23x512 is array(0 to 511) of std_logic_vector(22 downto 0);
	signal FFT_ValuesRe : memory23x512;
	signal FFT_ValuesIm : memory23x512;
	
	type memory24x5 is array(0 to 4) of signed(23 downto 0);
	signal FFT_IndexedRe : memory24x5;
	signal FFT_IndexedIm : memory24x5;
	
	signal outpacket : packet;
	
	type state is (
	stInitialize1,
	stInitialize2,
	stInit,
	stWaitForPing,
	stSampleAD,
	stLoadFFT0a,
	stLoadFFT0b,
	stUnloadFFT0a,
	stUnloadFFT0b,
	stCalcSignificantBits,
	stLoadFFT1a,
	stLoadFFT1b,
	stUnloadFFT1a,
	stUnloadFFT1b,
	stLoadFFT2a,
	stLoadFFT2b,
	stUnloadFFT2a,
	stUnloadFFT2b,
	stLoadFFT3a,
	stLoadFFT3b,
	stUnloadFFT3a,
	stUnloadFFT3b,
	stLoadFFT4a,
	stLoadFFT4b,
	stUnloadFFT4a,
	stUnloadFFT4b,
	stfftDone,
	stSerialInitByte1,
	stSerialInitByte2,
	stSerialInitByte3,
	stSerialInitByte4,
	stSendPayloadStartByte,
	stSendPayloadByte1,
	stSendPayloadByte2,
	stSendPayloadByte3,
	stSendPayloadByte4,
	stSendPayloadByte5,
	stSendPayloadByte6,
	stSendPayloadByte7,
	stSendPayloadByte8,
	stSendPayloadByte9,
	stSendPayloadByte10,
	stSendCheckSum,
	stSerialEnd);
	
	signal stCurrent : state := stInitialize1;
	signal nextState : state := stInitialize1;
	
	--FFT Siganls
	signal start : std_logic;
	signal xn_re : std_logic_vector (12 downto 0);
	signal xn_im : std_logic_vector (12 downto 0);
	signal fwd_inv : std_logic;
	signal fwd_inv_we : std_logic;
	signal rfd : std_logic;
	signal xn_index : std_logic_vector (8 downto 0);
	signal busy : std_logic;
	signal edone : std_logic;
	signal done : std_logic;
	signal dv : std_logic;
	signal unload : std_logic;
	signal xk_index : std_logic_vector (8 downto 0); 
	signal xk_re : std_logic_vector (22 downto 0);
	signal xk_im : std_logic_vector (22 downto 0);
		
begin
	
	UART: RS232RefComp port map (
		TXD 	=> TXD,
		RXD 	=> RXD,
		CLK 	=> clock,
		DBIN 	=> dbInSig,
		DBOUT	=> dbOutSig,
		RDA	=> rdaSig,
		TBE	=> tbeSig,	
		RD		=> rdSig,
		WR		=> wrSig,
		PE		=> peSig,
		FE		=> feSig,
		OE		=> oeSig,
		RST 	=> rstSig);
		
	MyFFT: fft port map (
		clk => clock,
		start => start,
		xn_re => xn_re,
		xn_im => xn_im,
		fwd_inv => fwd_inv,
		fwd_inv_we => fwd_inv_we,
		rfd => rfd,
		xn_index => xn_index,
		busy => busy,
		edone => edone,
		done => done,
		dv => dv,
		unload => unload,
		xk_index => xk_index,
		xk_re => xk_re,
		xk_im => xk_im);

	rstSig <= '0';
	
	process (clock)
	begin
		if (rising_edge(clock)) then
			stCurrent <= nextState;
		end if;
	end process;
	
	process (clock)
		variable sampleCounter : integer;
		variable doneFlag : std_logic;
		variable fftCounter : integer range 0 to 511;
		
		type array1 is array (0 to 4) of signed(63 downto 0);
		
		variable AD_Index : integer := 0;
		variable fft_power : signed(63 downto 0);
		variable fft_max_power : signed(63 downto 0);
		variable fft_max_index : integer;
		variable significantBit : integer;
		
		type memory12x5 is array (0 to 4) of std_logic_vector(11 downto 0);
		variable data_SLV12 : memory12x5;
		
	begin
		if (rising_edge(clock) and (stCurrent = nextState)) then
			if (fft_read = '1') then
				fft_ready <= '0';
			end if;
			
			if (tbeSig = '1') then
				case stCurrent is
					when stInitialize1 =>
						fwd_inv <= '0';
						fwd_inv_we <= '1';						
						nextState <= stInitialize2;
					
					when stInitialize2 =>
						fwd_inv_we <= '0';					
						nextState <= stInit;
					
					when stInit =>
						sampleCounter := 0;
						nextState <= stWaitForPing;
					
					when stWaitForPing =>
						case mode is
							when "00" => --Continuous Sampling
								nextState <= stSampleAD;
							
							when "01" => --Pings or'ed
								if ((ping0 = '1') or (ping1 = '1') or (ping2 = '1') or (ping3 = '1')) then
									nextState <= stSampleAD;
								end if;
								
							when "10" => --Pings and'ed
								if ((ping0 = '1') and (ping1 = '1') and (ping2 = '1') and (ping3 = '1')) then
									nextState <= stSampleAD;
								end if;
							
							when others =>
								--invalid mode
							
						end case;
					
					when stSampleAD =>
						--Sample ADC Channels
						if (analogDone = '1' and doneFlag = '0') then
							doneFlag := '1';
							AD0(sampleCounter) <= analog0;
							AD1(sampleCounter) <= analog1;
							AD2(sampleCounter) <= analog2;
							AD3(sampleCounter) <= analog3;
							AD4(sampleCounter) <= analog4;
							if (sampleCounter = 511) then
								--nextState <= stLoadFFT0a;
								nextState <= stSerialInitByte1;
							else
								sampleCounter := sampleCounter + 1;
							end if;								
						elsif (analogDone = '0') then
							doneFlag := '0';						
						end if;
					
					when stLoadFFT0a =>
						if (rfd = '0') then
							start <= '1';
							nextState <= stLoadFFT0b;
						end if;
					
					when stLoadFFT0b =>
						if (rfd = '1') then
							start <= '0';
							xn_re <= '0' & AD0(to_integer(unsigned(xn_index)));
							xn_im <= '0' & x"000";
							if (to_integer(unsigned(xn_index)) = 511) then
								nextState <= stUnloadFFT0a;
							end if;
						end if;
						
					when stUnloadFFT0a =>
						if ((rfd = '0') and (dv = '0')) then
							fft_max_power := to_signed(0, 64);
							unload <= '1';
							nextState <= stUnloadFFT0b;
						end if;
					
					when stUnloadFFT0b =>
						if (dv = '1') then
							unload <= '0';
							FFT_ValuesRe(to_integer(unsigned(xk_index))) <= xk_re;
							FFT_ValuesIm(to_integer(unsigned(xk_index))) <= xk_im;
							if ((to_integer(unsigned(xk_index)) > 5) and (to_integer(unsigned(xk_index)) < 256)) then
								fft_power := resize(signed(xk_re)*signed(xk_re) + signed(xk_im)*signed(xk_im), fft_power'length);
								if (fft_power >= fft_max_power) then
									fft_max_power := fft_power;
									fft_max_index := to_integer(unsigned(xk_index));
									FFT_IndexedRe(0) <= resize(signed(xk_re), FFT_IndexedRe(0)'length);
									FFT_IndexedIm(0) <= resize(signed(xk_im), FFT_IndexedIm(0)'length);
								end if;
							end if;
							
							if ((to_integer(unsigned(xk_index))) = 511) then
								nextState <= stCalcSignificantBits;
							end if;							
						end if;
						
					when stCalcSignificantBits =>
						significantBit := 11;
						for i in 11 to 63 loop
							if (fft_max_power(i) = '1') then
								significantBit := i;
							end if;
						end loop;
						nextState <= stLoadFFT1a;
						
					when stLoadFFT1a =>
						if (rfd = '0') then
							start <= '1';
							nextState <= stLoadFFT1b;
						end if;
					
					when stLoadFFT1b =>
						if (rfd = '1') then
							start <= '0';
							xn_re <= '0' & AD1(to_integer(unsigned(xn_index)));
							xn_im <= '0' & x"000";
							if (to_integer(unsigned(xn_index)) = 511) then
								nextState <= stUnloadFFT1a;
							end if;
						end if;
						
					when stUnloadFFT1a =>
						if ((rfd = '0') and (dv = '0')) then
							fft_max_power := to_signed(0, 64);
							unload <= '1';
							nextState <= stUnloadFFT1b;
						end if;
					
					when stUnloadFFT1b =>
						if (dv = '1') then
							unload <= '0';
							if (to_integer(unsigned(xk_index)) = fft_max_index) then
								FFT_IndexedRe(1) <= resize(signed(xk_re), FFT_IndexedRe(1)'length);
								FFT_IndexedIm(1) <= resize(signed(xk_im), FFT_IndexedIm(1)'length);
							end if;
							
							if ((to_integer(unsigned(xk_index))) = 511) then
								nextState <= stLoadFFT2a;
							end if;							
						end if;
						
					when stLoadFFT2a =>
						if (rfd = '0') then
							start <= '1';
							nextState <= stLoadFFT2b;
						end if;
					
					when stLoadFFT2b =>
						if (rfd = '1') then
							start <= '0';
							xn_re <= '0' & AD2(to_integer(unsigned(xn_index)));
							xn_im <= '0' & x"000";
							if (to_integer(unsigned(xn_index)) = 511) then
								nextState <= stUnloadFFT2a;
							end if;
						end if;
						
					when stUnloadFFT2a =>
						if ((rfd = '0') and (dv = '0')) then
							fft_max_power := to_signed(0, 64);
							unload <= '1';
							nextState <= stUnloadFFT2b;
						end if;
					
					when stUnloadFFT2b =>
						if (dv = '1') then
							unload <= '0';
							if (to_integer(unsigned(xk_index)) = fft_max_index) then
								FFT_IndexedRe(2) <= resize(signed(xk_re), FFT_IndexedRe(2)'length);
								FFT_IndexedIm(2) <= resize(signed(xk_im), FFT_IndexedIm(2)'length);
							end if;
							
							if ((to_integer(unsigned(xk_index))) = 511) then
								nextState <= stLoadFFT3a;
							end if;							
						end if;
						
					when stLoadFFT3a =>
						if (rfd = '0') then
							start <= '1';
							nextState <= stLoadFFT3b;
						end if;
					
					when stLoadFFT3b =>
						if (rfd = '1') then
							start <= '0';
							xn_re <= '0' & AD3(to_integer(unsigned(xn_index)));
							xn_im <= '0' & x"000";
							if (to_integer(unsigned(xn_index)) = 511) then
								nextState <= stUnloadFFT3a;
							end if;
						end if;
						
					when stUnloadFFT3a =>
						if ((rfd = '0') and (dv = '0')) then
							fft_max_power := to_signed(0, 64);
							unload <= '1';
							nextState <= stUnloadFFT3b;
						end if;
					
					when stUnloadFFT3b =>
						if (dv = '1') then
							unload <= '0';
							if (to_integer(unsigned(xk_index)) = fft_max_index) then
								FFT_IndexedRe(3) <= resize(signed(xk_re), FFT_IndexedRe(3)'length);
								FFT_IndexedIm(3) <= resize(signed(xk_im), FFT_IndexedIm(3)'length);
							end if;
							
							if ((to_integer(unsigned(xk_index))) = 511) then
								nextState <= stLoadFFT4a;
							end if;							
						end if;
					
					when stLoadFFT4a =>
						if (rfd = '0') then
							start <= '1';
							nextState <= stLoadFFT4b;
						end if;
					
					when stLoadFFT4b =>
						if (rfd = '1') then
							start <= '0';
							xn_re <= '0' & AD4(to_integer(unsigned(xn_index)));
							xn_im <= '0' & x"000";
							if (to_integer(unsigned(xn_index)) = 511) then
								nextState <= stUnloadFFT4a;
							end if;
						end if;
						
					when stUnloadFFT4a =>
						if ((rfd = '0') and (dv = '0')) then
							fft_max_power := to_signed(0, 64);
							unload <= '1';
							nextState <= stUnloadFFT4b;
						end if;
					
					when stUnloadFFT4b =>
						if (dv = '1') then
							unload <= '0';
							if (to_integer(unsigned(xk_index)) = fft_max_index) then
								FFT_IndexedRe(4) <= resize(signed(xk_re), FFT_IndexedRe(4)'length);
								FFT_IndexedIm(4) <= resize(signed(xk_im), FFT_IndexedIm(4)'length);
							end if;
							
							if ((to_integer(unsigned(xk_index))) = 511) then
								nextState <= stFFTDone;
							end if;							
						end if;
						
					when stFFTDone =>
						fft_index <= std_logic_vector(to_unsigned(fft_max_index, 24));
						fft0_re <= std_logic_vector(FFT_IndexedRe(0));
						fft0_im <= std_logic_vector(FFT_IndexedIm(0));
						fft1_re <= std_logic_vector(FFT_IndexedRe(1));
						fft1_im <= std_logic_vector(FFT_IndexedIm(1));
						fft2_re <= std_logic_vector(FFT_IndexedRe(2));
						fft2_im <= std_logic_vector(FFT_IndexedIm(2));
						fft3_re <= std_logic_vector(FFT_IndexedRe(3));
						fft3_im <= std_logic_vector(FFT_IndexedIm(3));
						fft4_re <= std_logic_vector(FFT_IndexedRe(4));
						fft4_im <= std_logic_vector(FFT_IndexedIm(4));
						fft_ready <= '1';
						nextState <= stSerialInitByte1;						
					
					when stSerialInitByte1 =>
						--Send start byte sequence
						dbInSig(7 downto 0) <= x"AA";
						wrSig <= '1';
						nextState <= stSerialInitByte2;
						
					when stSerialInitByte2 =>
						dbInSig(7 downto 0) <= x"AA";
						wrSig <= '1';
						nextState <= stSerialInitByte3;
						
					when stSerialInitByte3 =>
						dbInSig(7 downto 0) <= x"AA";
						wrSig <= '1';
						nextState <= stSerialInitByte4;
					
					when stSerialInitByte4 =>
						dbInSig(7 downto 0) <= x"AA";
						wrSig <= '1';
						AD_Index := 511;
						nextState <= stSendPayloadStartByte;
					
					when stSendPayloadStartByte =>
						dbInSig(7 downto 0) <= x"FF";
						outpacket(0) <= x"FF";
						wrSig <= '1';
						--Prepare Payload
						fft_power := resize(signed(FFT_ValuesRe(AD_Index))*signed(FFT_ValuesRe(AD_Index)) + signed(FFT_ValuesIm(AD_Index))*signed(FFT_ValuesIm(AD_Index)), fft_power'length);
						data_SLV12(0) := AD0(AD_Index);
						data_SLV12(1) := AD1(AD_Index);
						data_SLV12(2) := AD2(AD_Index);
						data_SLV12(3) := AD3(AD_Index);
						data_SLV12(4) := AD4(AD_Index);
--						data_SLV12(1) := std_logic_vector(fft_power(significantBit downto significantBit-11));
--						data_SLV12(2) := std_logic_vector(to_unsigned(AD_Index, 12));
--						data_SLV12(3) := x"000";
--						data_SLV12(4) := x"000";
						nextState <= stSendPayloadByte1;
						
					when stSendPayloadByte1 =>
						dbInSig <= x"0" & data_SLV12(0)(11 downto 8);
						outpacket(1) <= x"0" & data_SLV12(0)(11 downto 8);
						wrSig <= '1';
						nextState <= stSendPayloadByte2;
						
					when stSendPayloadByte2 =>
						dbInSig <= data_SLV12(0)(7 downto 0);
						outpacket(2) <= data_SLV12(0)(7 downto 0);
						wrSig <= '1';
						nextState <= stSendPayloadByte3;
						
					when stSendPayloadByte3 =>
						dbInSig <= x"0" & data_SLV12(1)(11 downto 8);
						outpacket(3) <= x"0" & data_SLV12(1)(11 downto 8);
						wrSig <= '1';
						nextState <= stSendPayloadByte4;
						
					when stSendPayloadByte4 =>
						dbInSig <= data_SLV12(1)(7 downto 0);
						outpacket(4) <= data_SLV12(1)(7 downto 0);
						wrSig <= '1';
						nextState <= stSendPayloadByte5;
						
					when stSendPayloadByte5 =>
						dbInSig <= x"0" & data_SLV12(2)(11 downto 8);
						outpacket(5) <= x"0" & data_SLV12(2)(11 downto 8);
						wrSig <= '1';
						nextState <= stSendPayloadByte6;
						
					when stSendPayloadByte6 =>
						dbInSig <= data_SLV12(2)(7 downto 0);
						outpacket(6) <= data_SLV12(2)(7 downto 0);
						wrSig <= '1';
						nextState <= stSendPayloadByte7;
						
					when stSendPayloadByte7 =>
						dbInSig <= x"0" & data_SLV12(3)(11 downto 8);
						outpacket(7) <= x"0" & data_SLV12(3)(11 downto 8);
						wrSig <= '1';
						nextState <= stSendPayloadByte8;
						
					when stSendPayloadByte8 =>
						dbInSig <= data_SLV12(3)(7 downto 0);
						outpacket(8) <= data_SLV12(3)(7 downto 0);
						wrSig <= '1';
						nextState <= stSendPayloadByte9;
					
					when stSendPayloadByte9 =>
						dbInSig <= x"0" & data_SLV12(4)(11 downto 8);
						outpacket(9) <= x"0" & data_SLV12(4)(11 downto 8);
						wrSig <= '1';
						nextState <= stSendPayloadByte10;
						
					when stSendPayloadByte10 =>
						dbInSig <= data_SLV12(4)(7 downto 0);
						outpacket(10) <= data_SLV12(4)(7 downto 0);
						wrSig <= '1';
						nextState <= stSendCheckSum;
					
					when stSendCheckSum =>
						dbInSig <= outpacket(0) xor outpacket(1) xor outpacket(2) xor outpacket(3) xor outpacket(4) xor outpacket(5) xor outpacket(6) xor outpacket(7) xor outpacket(8) xor outpacket(9) xor outpacket(10);
						wrSig <= '1';
						nextState <= stSerialEnd;
						
					when stSerialEnd =>
						if (AD_Index = 0) then
								nextState <= stInit;
						else
							AD_Index := AD_Index - 1;
							nextState <= stSendPayloadStartByte;
						end if;
				
				end case;
			else
				wrSig <= '0';
			end if;
		end if;
	end process;

end Behavioral;