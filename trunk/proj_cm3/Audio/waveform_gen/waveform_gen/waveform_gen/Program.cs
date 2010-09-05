/*
 * Crée par SharpDevelop.
 * Utilisateur: Seb
 * Date: 01/08/2010
 * Heure: 13:52
 * 
 * Pour changer ce modèle utiliser Outils | Options | Codage | Editer les en-têtes standards.
 */
using System;
using System.IO;

namespace waveform_gen
{
	class Program
	{
		static float[] Table;
		static int index;
		static int nb_waveform;
		
		// create a writer and open the file
        static TextWriter C_file;
		static TextWriter H_file;
		
		static int Taille;
		static float Taille_f;
		
		public static void Main(string[] args)
		{
			Taille = 64;
			Taille_f = (float)64.0;
			
			Table = new float[Taille];
			C_file = new StreamWriter("waveform.c");
			H_file = new StreamWriter("waveform.h");
			
			nb_waveform=0;
			
			C_file.WriteLine("/*");
			C_file.WriteLine(" * Copyright (C) INSA Toulouse");
			C_file.WriteLine(" * Author: Sebastien DI MERCURIO");
			C_file.WriteLine(" *");
			C_file.WriteLine(" * This file is part of MinOS.");
			C_file.WriteLine(" *");
			C_file.WriteLine(" * MinOS is free software; you can redistribute it and/or");
			C_file.WriteLine(" * modify it under the terms of the GNU General Public");
			C_file.WriteLine(" * License as published by the Free Software Foundation;");
			C_file.WriteLine(" * either version 2, or (at your option) any later version.");
			C_file.WriteLine(" *");
			C_file.WriteLine(" * MinOS is distributed in the hope that it will be useful,");
			C_file.WriteLine(" * but WITHOUT ANY WARRANTY; without even the implied");
			C_file.WriteLine(" * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR");
			C_file.WriteLine(" * PURPOSE.  See the GNU General Public License for more");
			C_file.WriteLine(" * details.");
			C_file.WriteLine(" *");
			C_file.WriteLine(" * You should have received a copy of the GNU General Public");
			C_file.WriteLine(" * License along with MinOS; see the file COPYING.  If not,");
			C_file.WriteLine(" * write to the Free Software Foundation, Inc., 51 Franklin Street,");
			C_file.WriteLine(" * Fifth Floor, Boston, MA 02110-1301, USA.");
			C_file.WriteLine(" */\n");
			
			C_file.WriteLine("#include \"stm_regs.h\"\n#include \"waveform.h\"\n");
			C_file.WriteLine("const u32 Waveforms[_WAVEFORMS_NB_][_WAVEFORMS_LENGTH_] =\n{");
			
			C_file.Write("\t{ // Triangle");
				nb_waveform++;
				
				for (index=0;index<Taille/2;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)(((float)index)*4096.0/(Taille_f/2))+",");
				}
				
				for (index=Taille/2;index>0;index--)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)(((float)index)*4096.0/(Taille_f/2))+",");
				}
			C_file.WriteLine("\n\t},");

			C_file.Write("\t{ // Sinus");
				nb_waveform++;
				
				for (index=0;index<Taille/2;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)((Math.Sin((float)index*Math.PI/64.0))*2048.0+2048.0)+",");
				}
				
				for (index=Taille/2;index<Taille;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)((Math.Sin((float)index*Math.PI/64.0))*2048.0+2048.0)+",");
				}
			C_file.WriteLine("\n\t},");
			
			C_file.Write("\t{ // Square");
				nb_waveform++;
				
				for (index=0;index<Taille/2;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write(((int)4096.0)+",");
				}
				
				for (index=Taille/2;index<Taille;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write(((int)0.0)+",");
				}
			C_file.WriteLine("\n\t},");
			
			C_file.Write("\t{ // Sawteeth");
				nb_waveform++;
				
				for (index=0;index<Taille/2;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)(((float)index)*4096.0/(Taille_f/2))+",");
				}
				
				for (index=0;index<Taille/2;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)(((float)index)*4096.0/(Taille_f/2))+",");
				}
			C_file.WriteLine("\n\t},");
			
			C_file.Write("\t{ // Half sinus");
				nb_waveform++;
				
				for (index=0;index<Taille/2;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)((Math.Sin((float)index*Math.PI/(Taille_f/2)))*2048.0+2048.0)+",");
				}
				
				for (index=Taille/2;index<Taille;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write(((int)0.0)+",");
				}
			C_file.WriteLine("\n\t},");
			
			C_file.Write("\t{ // Double positive sinus");
				nb_waveform++;
				
				for (index=0;index<Taille/2;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)((Math.Sin((float)index*Math.PI/(Taille_f/2)))*4096.0)+",");
				}
				
				for (index=0;index<Taille/2;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)((Math.Sin((float)index*Math.PI/(Taille_f/2)))*4096.0)+",");
				}
			C_file.WriteLine("\n\t},");
			
			C_file.Write("\t{ // Half sawteeth");
				nb_waveform++;
				
				for (index=0;index<Taille/2;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write((int)(((float)index)*4096.0/(Taille_f/2))+",");
				}
				
				for (index=Taille/2;index<Taille;index++)
				{
					if (index%8==0) C_file.Write("\n\t");
					
					C_file.Write(((int)0.0)+",");
				}
			C_file.WriteLine("\n\t},");
			
			C_file.WriteLine("};\n");
			// close the stream
            C_file.Close();
            
            H_file.WriteLine("/*");
			H_file.WriteLine(" * Copyright (C) INSA Toulouse");
			H_file.WriteLine(" * Author: Sebastien DI MERCURIO");
			H_file.WriteLine(" *");
			H_file.WriteLine(" * This file is part of MinOS.");
			H_file.WriteLine(" *");
			H_file.WriteLine(" * MinOS is free software; you can redistribute it and/or");
			H_file.WriteLine(" * modify it under the terms of the GNU General Public");
			H_file.WriteLine(" * License as published by the Free Software Foundation;");
			H_file.WriteLine(" * either version 2, or (at your option) any later version.");
			H_file.WriteLine(" *");
			H_file.WriteLine(" * MinOS is distributed in the hope that it will be useful,");
			H_file.WriteLine(" * but WITHOUT ANY WARRANTY; without even the implied");
			H_file.WriteLine(" * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR");
			H_file.WriteLine(" * PURPOSE.  See the GNU General Public License for more");
			H_file.WriteLine(" * details.");
			H_file.WriteLine(" *");
			H_file.WriteLine(" * You should have received a copy of the GNU General Public");
			H_file.WriteLine(" * License along with MinOS; see the file COPYING.  If not,");
			H_file.WriteLine(" * write to the Free Software Foundation, Inc., 51 Franklin Street,");
			H_file.WriteLine(" * Fifth Floor, Boston, MA 02110-1301, USA.");
			H_file.WriteLine(" */\n");
			
			H_file.WriteLine("#define _WAVEFORMS_NB_ "+nb_waveform+"\n#define _WAVEFORMS_LENGTH_ 128\n");
			H_file.WriteLine("extern const u32 Waveforms[_WAVEFORMS_NB_][_WAVEFORMS_LENGTH_];\n");
			H_file.Close();
			
			Console.Write("Press any key to continue . . . ");
			Console.ReadKey(true);
		}
	}
}