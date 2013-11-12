library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

package dPotController is
	component dPotControllerComp is
	port (	
		clock : in std_logic;
		
		SCL0 : inout std_logic;
		SDA0 : inout std_logic;
		
		SCL1 : inout std_logic;
		SDA1 : inout std_logic;

		adderOut0 : in std_logic_vector (7 downto 0);
		vgnA0 : in std_logic_vector (7 downto 0);
		vgnB0 : in std_logic_vector (7 downto 0);
		vref0 : in std_logic_vector (7 downto 0);
		
		adderOut1 : in std_logic_vector (7 downto 0);
		vgnA1 : in std_logic_vector (7 downto 0);
		vgnB1 : in std_logic_vector (7 downto 0);
		vref1 : in std_logic_vector (7 downto 0);

		adderOut2 : in std_logic_vector (7 downto 0);
		vgnA2 : in std_logic_vector (7 downto 0);
		vgnB2 : in std_logic_vector (7 downto 0);
		vref2 : in std_logic_vector (7 downto 0);

		adderOut3 : in std_logic_vector (7 downto 0);
		vgnA3 : in std_logic_vector (7 downto 0);
		vgnB3 : in std_logic_vector (7 downto 0);
		vref3 : in std_logic_vector (7 downto 0);

		adderOut4 : in std_logic_vector (7 downto 0);
		vgnA4 : in std_logic_vector (7 downto 0);
		vgnB4 : in std_logic_vector (7 downto 0);
		vref4 : in std_logic_vector (7 downto 0)
	);
	end component dPotControllerComp;
end package dPotController;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;
use work.DS1803.all;

entity dPotControllerComp is
	port (	
		clock : in std_logic;
		
		SCL0 : inout std_logic;
		SDA0 : inout std_logic;
		
		SCL1 : inout std_logic;
		SDA1 : inout std_logic;

		adderOut0 : in std_logic_vector (7 downto 0);
		vgnA0 : in std_logic_vector (7 downto 0);
		vgnB0 : in std_logic_vector (7 downto 0);
		vref0 : in std_logic_vector (7 downto 0);
		
		adderOut1 : in std_logic_vector (7 downto 0);
		vgnA1 : in std_logic_vector (7 downto 0);
		vgnB1 : in std_logic_vector (7 downto 0);
		vref1 : in std_logic_vector (7 downto 0);

		adderOut2 : in std_logic_vector (7 downto 0);
		vgnA2 : in std_logic_vector (7 downto 0);
		vgnB2 : in std_logic_vector (7 downto 0);
		vref2 : in std_logic_vector (7 downto 0);

		adderOut3 : in std_logic_vector (7 downto 0);
		vgnA3 : in std_logic_vector (7 downto 0);
		vgnB3 : in std_logic_vector (7 downto 0);
		vref3 : in std_logic_vector (7 downto 0);

		adderOut4 : in std_logic_vector (7 downto 0);
		vgnA4 : in std_logic_vector (7 downto 0);
		vgnB4 : in std_logic_vector (7 downto 0);
		vref4 : in std_logic_vector (7 downto 0)
	);
end dPotControllerComp;

architecture Behavioral of dPotControllerComp is

	signal ds1803Interface0_Value : std_logic_vector(7 downto 0);
	signal ds1803Interface0_ChipSel : std_logic_vector(2 downto 0);
	signal ds1803Interface0_PotSel : std_logic_vector(1 downto 0);
	signal ds1803Interface0_setPot, ds1803Interface0_potSet : std_logic;
	
	signal ds1803Interface1_Value : std_logic_vector(7 downto 0);
	signal ds1803Interface1_ChipSel : std_logic_vector(2 downto 0);
	signal ds1803Interface1_PotSel : std_logic_vector(1 downto 0);
	signal ds1803Interface1_setPot, ds1803Interface1_potSet : std_logic;

	type state is (
	pot0a,
	pot0b,
	pot1a,
	pot1b,
	pot2a,
	pot2b,
	pot3a,
	pot3b,
	pot4a,
	pot4b,
	pot5a,
	pot5b,
	pot6a,
	pot6b,
	pot7a,
	pot7b,
	pot8a,
	pot8b,
	pot9a,
	pot9b,
	pot10a,
	pot10b,
	pot11a,
	pot11b,
	pot12a,
	pot12b,
	pot13a,
	pot13b,
	pot14a,
	pot14b,
	pot15a,
	pot15b,
	pot16a,
	pot16b,
	pot17a,
	pot17b,
	pot18a,
	pot18b,
	pot19a,
	pot19b,	
	done
	);

	signal currentState : state := pot0a;
	signal nextState : state := pot0a;

begin

	ds1803Interface0: ds1803Comp port map
	(
		clk => clock,
		ChipSelect => ds1803Interface0_ChipSel,
		POTSelect => ds1803Interface0_PotSel,
		DataIn => ds1803Interface0_Value,
		SCL => SCL0,
		SDA => SDA0,
		setPot => ds1803Interface0_setPot,
		potSet => ds1803Interface0_potSet
	);
	
	ds1803Interface1: ds1803Comp port map
	(
		clk => clock,
		ChipSelect => ds1803Interface1_ChipSel,
		POTSelect => ds1803Interface1_PotSel,
		DataIn => ds1803Interface1_Value,
		SCL => SCL1,
		SDA => SDA1,
		setPot => ds1803Interface1_setPot,
		potSet => ds1803Interface1_potSet
	);

	process(clock)
	begin
		if (rising_edge(clock)) then
			currentState <= nextState;
		end if;
	end process;

	process(clock, currentState)
		variable counter : integer := 0;
	begin
		if(rising_edge(clock)) then
			
			if (currentState = nextState) then			
				case currentState is 
					when pot0a =>
						ds1803Interface0_ChipSel <= "000";
						ds1803Interface0_PotSel <= "01";
						ds1803Interface0_Value <= vgnB0;
						ds1803Interface0_setPot <= '1';
						nextState <= pot0b;
						
					when pot0b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot1a;
						end if;
						
						
					when pot1a =>
						ds1803Interface0_ChipSel <= "000";
						ds1803Interface0_PotSel <= "10";
						ds1803Interface0_Value <= vgnA0;
						ds1803Interface0_setPot <= '1';
						nextState <= pot1b;
						
					when pot1b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot2a;
						end if;
					
					when pot2a =>
						ds1803Interface0_ChipSel <= "001";
						ds1803Interface0_PotSel <= "01";
						ds1803Interface0_Value <= adderOut0;
						ds1803Interface0_setPot <= '1';
						nextState <= pot2b;
						
					when pot2b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot3a;
						end if;
					
					when pot3a =>
						ds1803Interface0_ChipSel <= "001";
						ds1803Interface0_PotSel <= "10";
						ds1803Interface0_Value <= vref0;
						ds1803Interface0_setPot <= '1';
						nextState <= pot3b;
						
					when pot3b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot4a;
						end if;
					
					when pot4a =>
						ds1803Interface0_ChipSel <= "010";
						ds1803Interface0_PotSel <= "01";
						ds1803Interface0_Value <= vgnB1;
						ds1803Interface0_setPot <= '1';
						nextState <= pot4b;
						
					when pot4b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot5a;
						end if;
					
					when pot5a =>
						ds1803Interface0_ChipSel <= "010";
						ds1803Interface0_PotSel <= "10";
						ds1803Interface0_Value <= vgnA1;
						ds1803Interface0_setPot <= '1';
						nextState <= pot5b;
					
					when pot5b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot6a;
						end if;					

					when pot6a =>
						ds1803Interface0_ChipSel <= "011";
						ds1803Interface0_PotSel <= "01";
						ds1803Interface0_Value <= adderOut1;
						ds1803Interface0_setPot <= '1';
						nextState <= pot6b;
					
					when pot6b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot7a;
						end if;	
					
					when pot7a =>
						ds1803Interface0_ChipSel <= "011";
						ds1803Interface0_PotSel <= "10";
						ds1803Interface0_Value <= vref1;
						ds1803Interface0_setPot <= '1';
						nextState <= pot7b;
					
					when pot7b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot8a;
						end if;	
					
					when pot8a =>
						ds1803Interface0_ChipSel <= "100";
						ds1803Interface0_PotSel <= "01";
						ds1803Interface0_Value <= vgnB2;
						ds1803Interface0_setPot <= '1';
						nextState <= pot8b;
					
					when pot8b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot9a;
						end if;	
					
					when pot9a =>
						ds1803Interface0_ChipSel <= "100";
						ds1803Interface0_PotSel <= "10";
						ds1803Interface0_Value <= vgnA2;
						ds1803Interface0_setPot <= '1';
						nextState <= pot9b;
					
					when pot9b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot10a;
						end if;						
						
					when pot10a =>
						ds1803Interface0_ChipSel <= "101";
						ds1803Interface0_PotSel <= "01";
						ds1803Interface0_Value <= adderOut2;
						ds1803Interface0_setPot <= '1';
						nextState <= pot10b;
					
					when pot10b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot11a;
						end if;
					
					when pot11a =>
						ds1803Interface0_ChipSel <= "101";
						ds1803Interface0_PotSel <= "10";
						ds1803Interface0_Value <= vref2;
						ds1803Interface0_setPot <= '1';
						nextState <= pot11b;
					
					when pot11b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot12a;
						end if;
					
					when pot12a =>
						ds1803Interface0_ChipSel <= "110";
						ds1803Interface0_PotSel <= "01";
						ds1803Interface0_Value <= vgnB3;
						ds1803Interface0_setPot <= '1';
						nextState <= pot12b;
					
					when pot12b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot13a;
						end if;
					
					when pot13a =>
						ds1803Interface0_ChipSel <= "110";
						ds1803Interface0_PotSel <= "10";
						ds1803Interface0_Value <= vgnA3;
						ds1803Interface0_setPot <= '1';
						nextState <= pot13b;
					
					when pot13b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot14a;
						end if;

					when pot14a =>
						ds1803Interface0_ChipSel <= "111";
						ds1803Interface0_PotSel <= "01";
						ds1803Interface0_Value <= adderOut3;
						ds1803Interface0_setPot <= '1';
						nextState <= pot14b;
						
					when pot14b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot15a;
						end if;
					
					when pot15a =>
						ds1803Interface0_ChipSel <= "111";
						ds1803Interface0_PotSel <= "10";
						ds1803Interface0_Value <= vref3;
						ds1803Interface0_setPot <= '1';
						nextState <= pot15b;
						
					when pot15b =>
						if (ds1803Interface0_potSet = '1') then
							ds1803Interface0_setPot <= '0';
							nextState <= pot16a;
							--nextState <= done;
						end if;
					
					when pot16a =>
						ds1803Interface1_ChipSel <= "000";
						ds1803Interface1_PotSel <= "01";
						ds1803Interface1_Value <= vgnB4;
						ds1803Interface1_setPot <= '1';
						nextState <= pot16b;
						
					when pot16b =>
						if (ds1803Interface1_potSet = '1') then
							ds1803Interface1_setPot <= '0';
							nextState <= pot17a;
						end if;
					
					when pot17a =>
						ds1803Interface1_ChipSel <= "000";
						ds1803Interface1_PotSel <= "10";
						ds1803Interface1_Value <= vgnA4;
						ds1803Interface1_setPot <= '1';
						nextState <= pot17b;
					
					when pot17b =>
						if (ds1803Interface1_potSet = '1') then
							ds1803Interface1_setPot <= '0';
							nextState <= pot18a;
						end if;
					
					when pot18a =>
						ds1803Interface1_ChipSel <= "001";
						ds1803Interface1_PotSel <= "01";
						ds1803Interface1_Value <= adderOut4;
						ds1803Interface1_setPot <= '1';
						nextState <= pot18b;
						
					when pot18b =>
						if (ds1803Interface1_potSet = '1') then
							ds1803Interface1_setPot <= '0';
							nextState <= pot19a;
						end if;
						
					when pot19a =>
						ds1803Interface1_ChipSel <= "001";
						ds1803Interface1_PotSel <= "10";
						ds1803Interface1_Value <= vref4;
						ds1803Interface1_setPot <= '1';
						nextState <= pot19b;
						
					when pot19b =>
						if (ds1803Interface1_potSet = '1') then
							ds1803Interface1_setPot <= '0';
							nextState <= done;
						end if;

					when done =>
						if (counter < 25000000) then
							counter := counter + 1;
						else
							counter := 0;
							nextState <= pot0a;
						end if;					
					
				end case;
			end if;
		end if;

	end process;
	
end Behavioral;