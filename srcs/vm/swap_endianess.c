/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_endianess.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:46:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/22 20:37:06 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>

uint32_t	swap32_endian(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

uint16_t	swap16_endian(uint16_t val)
{
	val = (uint16_t)(val << 8) | (uint16_t)(val >> 8);
	return (val);
}
