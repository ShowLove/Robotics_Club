-- MCU Packet Receiver Transmitter (MCUPRT) Component
-- Robotics Club at UCF
-- Jonathan Mohlenhoff - jmohlenh@ist.ucf.edu

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD;
use IEEE.std_logic_unsigned.all;

package MCUPRT is
	component mcuprtComp is
	Port
	(
		TXD		: out std_logic := '1';
		RXD		: in std_logic;
		clock		: in	std_logic;
		
		commandRx	: out std_logic_vector (7 downto 0) := x"00";
		dataRx	: out std_logic_vector (31 downto 0) := x"00000000";
		packetRx	: out std_logic := '0';
		packetRd	: in std_logic;
		
		commandTx	: in std_logic_vector (7 downto 0);
		dataTx	: in std_logic_vector (31 downto 0);
		packetTx	: in std_logic;
		packetSent	: out std_logic := '1'
	);
	end component mcuprtComp;
end package MCUPRT;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD;
use IEEE.std_logic_unsigned.all;
use work.RS232.all;

entity mcuprtComp is
	Port
	(
		TXD		: out std_logic := '1';
		RXD		: in std_logic;
		clock		: in	std_logic;
		
		commandRx	: out std_logic_vector (7 downto 0) := x"00";
		dataRx	: out std_logic_vector (31 downto 0) := x"00000000";
		packetRx	: out std_logic := '0';
		packetRd	: in std_logic;
		
		commandTx	: in std_logic_vector (7 downto 0);
		dataTx	: in std_logic_vector (31 downto 0);
		packetTx	: in std_logic;
		packetSent	: out std_logic := '1'
	);
end mcuprtComp;

architecture Behavioral of mcuprtComp is

	type packetRxState is (waitForStartBytes, getHeader, getDataByte0, getDataByte1, getDataByte2, getDataByte3, processChecksum);
	signal packetRxStateCurrent: packetRxState := waitForStartBytes;
	signal packetRxStateNext: packetRxState;
	
	type receiveState is (storePreviousByte, storeCurrentByte, processCurrentByte);
	signal receiveStateCurrent : receiveState := storePreviousByte;
	signal receiveStateNext : receiveState;
	
	type packetTxState is (waitForPacket, txStartByte1, txStartByte2, txCommand, txDataByte0, txDataByte1, txDataByte2, txDataByte3, txChecksum, txDone);
	signal packetTxStateCurrent : packetTxState := waitForPacket;
	signal packetTxStateNext : packetTxState;
	
	signal currentByte, previousByte: std_logic_vector(7 downto 0);
	signal startByte1 : std_logic_vector (7 downto 0) := x"23"; -- '#'
	signal startByte2 : std_logic_vector (7 downto 0) := x"25"; -- '%'
	
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

	rstSig <= '0';
	
	process (clock)
	begin
		if (rising_edge(clock)) then
			receiveStateCurrent <= receiveStateNext;
			packetRxStateCurrent <= packetRxStateNext;
			packetTxStateCurrent <= packetTxStateNext;
		end if;
	end process;
	
	rxProcess: process (receiveStateCurrent, rdaSig, dbOutSig)
	variable calculatedChecksum : std_logic_vector (7 downto 0);
	variable tempCommandByte : std_logic_vector (7 downto 0);
	variable tempDataByte0, tempDataByte1, tempDataByte2, tempDataByte3 : std_logic_vector (7 downto 0);
	variable tempChecksum : std_logic_vector (7 downto 0);
	variable goodPacketsRx : std_logic_vector (7 downto 0) := x"00";
	variable badPacketsRx : std_logic_vector (7 downto 0) := x"00";
	begin
		if (rising_edge(clock)) then
			if (receiveStateCurrent = receiveStateNext) then
				case receiveStateCurrent is
					when storePreviousByte =>
						if (packetRd = '1') then
							packetRx <= '0';
						end if;
						rdSig <= '0';
						if (rdaSig = '1') then -- if data is ready to be read
							previousByte <= currentByte;
							receiveStateNext <= storeCurrentByte;
						else
							receiveStateNext <= storePreviousByte;
						end if;
						
					when storeCurrentByte =>
						currentByte <= dbOutSig;
						rdSig <= '1'; -- strobe rdSig to indicate that data has been read
						receiveStateNext <= processCurrentByte;

					when processCurrentByte =>
						rdSig <= '0';
						receiveStateNext <= storePreviousByte;
						
						--Process Byte FSM
						if ((previousByte = startByte1) and (currentByte = startByte2)) then
							packetRxStateNext <= getHeader;
						else
							if (packetRxStateCurrent = packetRxStateNext) then
								case packetRxStateCurrent is
									when waitForStartBytes =>
										packetRxStateNext <= waitForStartBytes;
										
									when getHeader =>
										tempCommandByte := currentByte;
										packetRxStateNext <= getDataByte0;
									
									when getDataByte0 =>
										tempDataByte0 := currentByte;
										packetRxStateNext <= getDataByte1;
									
									when getDataByte1 =>
										tempDataByte1 := currentByte;
										packetRxStateNext <= getDataByte2;
										
									when getDataByte2 =>
										tempDataByte2 := currentByte;
										packetRxStateNext <= getDataByte3;
										
									when getDataByte3 =>
										tempDataByte3 := currentByte;
										packetRxStateNext <= processChecksum;
									
									when processChecksum =>
										tempChecksum := currentByte;
										calculatedChecksum := startByte1 + startByte2 + tempCommandByte + tempDataByte0 + tempDataByte1 + tempDataByte2 + tempDataByte3;
										if (tempChecksum = calculatedChecksum) then
											goodPacketsRx := goodPacketsRx + x"01";
											commandRx <= tempCommandByte;
											dataRx (7 downto 0) <= tempDataByte0;
											dataRx (15 downto 8) <= tempDataByte1;
											dataRx (23 downto 16) <= tempDataByte2;
											dataRx (31 downto 24) <= tempDataByte3;
											packetRx <= '1';
										else
											badPacketsRx := badPacketsRx + x"01";
										end if;
										packetRxStateNext <= waitForStartBytes;
										
								end case; --rx packet fsm
							end if; --rx packet state synchronization
						end if;
				end case; --receive byte fsm
			end if; --receive byte state synchronization
		end if; --clock synchronization
	end process;
	
	txProcess: process (clock)
	variable tempTxCommand, tempTxDataByte0, tempTxDataByte1, tempTxDataByte2, tempTxDataByte3, tempTxChecksum: std_logic_vector (7 downto 0);
	variable txCounter : integer;
	begin
		if (rising_edge(clock)) then
			if (tbeSig = '1') then
				case packetTxStateCurrent is
					when waitForPacket =>
						txCounter := 0;
						if (packetTx = '1') then
							packetSent <= '0';
							
--							Forced PONG Message
--							tempTxCommand := x"84";
--							tempTxDataByte0 := x"50";
--							tempTxDataByte1 := x"4F";
--							tempTxDataByte2 := x"4E";
--							tempTxDataByte3 := x"47";
--							tempTxChecksum := startByte1 + startByte2 + x"84" + x"50" + x"4F" + x"4E" + x"47";

							tempTxCommand := commandTx;
							tempTxDataByte0 := dataTx (7 downto 0);
							tempTxDataByte1 := dataTx (15 downto 8);
							tempTxDataByte2 := dataTx (23 downto 16);
							tempTxDataByte3 := dataTx (31 downto 24);
							tempTxChecksum := startByte1 + startByte2 + commandTx + dataTx(7 downto 0) + dataTx(15 downto 8) + dataTx(23 downto 16) + dataTx(31 downto 24);
							packetTxStateNext <= txStartByte1;
						else
							packetTxStateNext <= waitForPacket;
						end if;
						
					when txStartByte1 =>
						dbInSig <= startByte1;
						wrSig <= '1';
						packetTxStateNext <= txStartByte2;
						
					when txStartByte2 =>
						dbInSig <= startByte2;
						wrSig <= '1';
						packetTxStateNext <= txCommand;
					
					when txCommand =>
						dbInSig <= tempTxCommand;
						wrSig <= '1';
						packetTxStateNext <= txDataByte0;
						
					when txDataByte0 =>
						dbInSig <= tempTxDataByte0;
						wrSig <= '1';
						packetTxStateNext <= txDataByte1;
						
					when txDataByte1 =>
						dbInSig <= tempTxDataByte1;
						wrSig <= '1';
						packetTxStateNext <= txDataByte2;
						
					when txDataByte2 =>
						dbInSig <= tempTxDataByte2;
						wrSig <= '1';
						packetTxStateNext <= txDataByte3;
					
					when txDataByte3 =>
						dbInSig <= tempTxDataByte3;
						wrSig <= '1';
						packetTxStateNext <= txChecksum;
						
					when txChecksum =>
						dbInSig <= tempTxChecksum;
						wrSig <= '1';
						packetSent <= '1';
						packetTxStateNext <= waitForPacket;
						
					when txDone =>
						txCounter := txCounter + 1;
						if (txCounter > 250000) then
							packetTxStateNext <= waitForPacket;
							packetSent <= '1';
						end if;						
				end case;
			else
				wrSig <= '0';
			end if;
		end if;
	end process;
	
end Behavioral;