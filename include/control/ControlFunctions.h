//
// Created by dzing on 03/02/2021.
//

#ifndef VISORV3_CONTROLFUNCTIONS_H
#define VISORV3_CONTROLFUNCTIONS_H

#include "Packets.h"

/**
 * Endpoint for processing incoming packets
 * @param data Packet to process
 */
void processIncomingPacket(const std::shared_ptr<ServerBoundPacket> &data);

void processIncomingS03(const shared_ptr<S03Handshake> &pointer);

void processIncomingS05(const shared_ptr<S05RequestSensor> &pointer);

void processIncomingS09(const std::shared_ptr<S09DrawUpdate> &pointer);

void processIncomingS0C(const std::shared_ptr<S0CFileOperation> &pointer);

void processIncomingS0D(const std::shared_ptr<S0DFileUpload> &pointer);

void processIncomingS0E(const shared_ptr<S0EControlSet> &pointer);

#endif //VISORV3_CONTROLFUNCTIONS_H
